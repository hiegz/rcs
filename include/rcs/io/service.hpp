#ifndef RCS_IO_SERVICE_HPP
#define RCS_IO_SERVICE_HPP

#include <rcs/co/awaitable.hpp>

#include <rcs/execution/executor.hpp>
#include <rcs/execution/inline_executor.hpp>

#include <rcs/io/token.hpp>
#include <rcs/io/uring/cqe.hpp>
#include <rcs/io/uring/cqr.hpp>
#include <rcs/io/uring/flags.hpp>
#include <rcs/io/uring/op.hpp>
#include <rcs/io/uring/params.hpp>
#include <rcs/io/uring/setup.hpp>
#include <rcs/io/uring/sqe.hpp>
#include <rcs/io/uring/sqr.hpp>

#include <rcs/system/handle.hpp>

#include <atomic>
#include <memory>
#include <mutex>
#include <utility>

#include <cassert>
#include <cstdint>

struct sockaddr;

namespace rcs::io {

///
/// @brief   Asynchronous I/O service.
///
/// @details Facilitates asynchronous execution of I/O operations
///          with C++ coroutine integration.
///
template <rcs::execution::executor TExecutorType>
class service {
  public:
    /// @brief Executor type.
    using executor_t = TExecutorType;

  public:
    service(const service &)                     = delete;
    auto operator=(const service &) -> service & = delete;

  public:
    /// @brief Assign from an existing service instance.
    auto operator=(service &&)
        -> service & = default;

    /// @brief Construct from an existing service instance.
    service(service &&) = default;

    /// @brief Construct an asynchronous I/O service.
    service(service::executor_t &&executor, std::uint32_t bandwidth);

    /// @brief Default destructor.
    ~service() = default;

  public:
    /// @brief Check whether the service is idle.
    auto idle() const -> bool;

    /// @brief Check whether the service is busy.
    auto busy() const -> bool;

    /// @brief Get the number of pending I/O operations.
    auto pending() const -> std::uint32_t;

    /// @brief Get the number of I/O operations that can be executed concurrently.
    auto bandwidth() const -> std::uint32_t;

  public:
    /// @brief Accept a connection on a socket.
    auto accept(std::int32_t descriptor, struct ::sockaddr &address, std::uint32_t size)
        -> rcs::co::awaitable<std::int32_t>;

    /// @brief Initiate a connection to a socket.
    auto connect(std::int32_t descriptor, const struct ::sockaddr &address, std::uint32_t size)
        -> rcs::co::awaitable<std::int32_t>;

    /// @brief Read from a file descriptor into a specified buffer.
    auto read(std::int32_t descriptor, void *buffer, std::uint32_t size, std::uint64_t offset = 0)
        -> rcs::co::awaitable<std::int32_t>;

    /// @brief Write to a file descriptor from a specified buffer.
    auto write(std::int32_t descriptor, const void *buffer, std::uint32_t size, std::uint64_t offset = 0)
        -> rcs::co::awaitable<std::int32_t>;

  public:
    /// @brief Run the event processing loop to execute at most one handler.
    void run_one();

    /// @brief Run the event processing loop.
    void run();

  private:
    /// @brief Default constructor.
    service() = default;

  private:
    /// @brief Utilized executor.
    service::executor_t m_executor{};

    /// @brief io_uring instance identifier.
    rcs::system::handle m_handle{-1};

  private:
    struct sq_t {
        rcs::io::uring::sqr         r;
        std::unique_ptr<std::mutex> mutex =
            std::make_unique<std::mutex>();
    };

    /// @brief Submission queue.
    struct service::sq_t m_sq = {};

  private:
    struct cq_t {
        rcs::io::uring::cqr         r;
        std::unique_ptr<std::mutex> mutex =
            std::make_unique<std::mutex>();
    };

    /// @brief Completion queue.
    struct service::cq_t m_cq = {};

  private:
    /// @brief Number of pending operations.
    std::atomic<std::uint32_t> m_pending = {0};

    /// @brief Number of I/O operations that can be executed concurrently.
    std::atomic<std::uint32_t> m_bandwidth = {0};
};

} // namespace rcs::io

template <rcs::execution::executor TExecutorType>
rcs::io::service<TExecutorType>::service(service::executor_t &&executor, std::uint32_t bandwidth)
    : m_executor(std::forward<service::executor_t>(executor)), m_bandwidth(bandwidth) {
    rcs::io::uring::params params;

    params.flags |= rcs::io::uring::SETUP_NO_SQARRAY;
    params.flags |= rcs::io::uring::SETUP_CQSIZE;
    params.flags |= rcs::io::uring::SETUP_CLAMP;

    params.cq_capacity = m_bandwidth.load();
    m_handle           = rcs::system::handle(
        rcs::io::uring::setup(m_bandwidth.load(), params));

    m_sq.r = rcs::io::uring::sqr(m_handle.descriptor(), params);
    m_cq.r = rcs::io::uring::cqr(m_handle.descriptor(), params);
}

template <rcs::execution::executor TExecutorType>
auto rcs::io::service<TExecutorType>::idle()
    const -> bool { return service::pending() == 0; }

template <rcs::execution::executor TExecutorType>
auto rcs::io::service<TExecutorType>::busy()
    const -> bool { return service::pending() == service::bandwidth(); }

template <rcs::execution::executor TExecutorType>
auto rcs::io::service<TExecutorType>::pending()
    const -> std::uint32_t { return m_pending.load(); }

template <rcs::execution::executor TExecutorType>
auto rcs::io::service<TExecutorType>::bandwidth()
    const -> std::uint32_t { return m_bandwidth.load(); }

template <rcs::execution::executor TExecutorType>
auto rcs::io::service<TExecutorType>::accept(
    std::int32_t descriptor, struct ::sockaddr &address, std::uint32_t size)
    -> rcs::co::awaitable<std::int32_t> {
    assert(not busy());

    struct awaiter {
        rcs::io::service<TExecutorType>             *service = nullptr;
        rcs::io::token<int, std::coroutine_handle<>> token   = {-1, nullptr};

        std::int32_t       descriptor = -1;
        struct ::sockaddr *address    = nullptr;
        std::uint32_t      size       = 0;

        auto await_ready() const -> bool { return false; }
        auto await_resume() -> int { return token.result; };

        void await_suspend(std::coroutine_handle<> caller) {
            service->m_executor.execute([caller, this]() {
                const std::unique_lock<std::mutex> sql(*service->m_sq.mutex);
                rcs::io::uring::sqr               *sqr = &service->m_sq.r;
                rcs::io::uring::sqe               *sqe = &sqr->next();

                sqe->opcode     = rcs::io::uring::op::accept;
                sqe->descriptor = descriptor;
                sqe->address    = address;
                sqe->addrlen2   = &size;

                token.result       = -1;
                token.continuation = caller;
                sqe->token         = reinterpret_cast<std::uint64_t>(&token);

                sqr->submit();
                service->m_pending.fetch_add(1);
            });
        }
    };

    const std::int32_t ret = co_await awaiter{
        .service    = this,
        .descriptor = descriptor,
        .address    = &address,
        .size       = size};

    co_return ret;
}

/// @brief Initiate a connection to a socket.
template <rcs::execution::executor TExecutorType>
auto rcs::io::service<TExecutorType>::connect(
    std::int32_t descriptor, const struct ::sockaddr &address, std::uint32_t size)
    -> rcs::co::awaitable<std::int32_t> {
    assert(not busy());

    struct awaiter {
        rcs::io::service<TExecutorType>             *service = nullptr;
        rcs::io::token<int, std::coroutine_handle<>> token   = {-1, nullptr};

        std::int32_t             descriptor = -1;
        const struct ::sockaddr *address    = nullptr;
        std::uint32_t            size       = 0;

        auto await_ready() const -> bool { return false; }
        auto await_resume() -> int { return token.result; };

        void await_suspend(std::coroutine_handle<> caller) {
            service->m_executor.execute([caller, this]() {
                const std::unique_lock<std::mutex> sql(*service->m_sq.mutex);
                rcs::io::uring::sqr               *sqr = &service->m_sq.r;
                rcs::io::uring::sqe               *sqe = &sqr->next();

                sqe->opcode     = rcs::io::uring::op::connect;
                sqe->descriptor = descriptor;
                sqe->address    = const_cast<struct ::sockaddr *>(address);
                sqe->addrlen    = size;

                token.result       = -1;
                token.continuation = caller;
                sqe->token         = reinterpret_cast<std::uint64_t>(&token);

                sqr->submit();
                service->m_pending.fetch_add(1);
            });
        }
    };

    const std::int32_t ret = co_await awaiter{
        .service    = this,
        .descriptor = descriptor,
        .address    = &address,
        .size       = size};

    co_return ret;
}

template <rcs::execution::executor TExecutorType>
auto rcs::io::service<TExecutorType>::read(
    std::int32_t descriptor, void *buffer, std::uint32_t size, std::uint64_t offset)
    -> rcs::co::awaitable<std::int32_t> {
    assert(not busy());

    struct awaiter {
        rcs::io::service<TExecutorType>             *service = nullptr;
        rcs::io::token<int, std::coroutine_handle<>> token   = {-1, nullptr};

        std::int32_t  descriptor = -1;
        void         *buffer     = nullptr;
        std::uint32_t size       = 0;
        std::uint64_t offset     = 0;

        auto await_ready() const -> bool { return false; }
        auto await_resume() -> int { return token.result; };

        void await_suspend(std::coroutine_handle<> caller) {
            service->m_executor.execute([caller, this]() {
                const std::unique_lock<std::mutex> sql(*service->m_sq.mutex);
                rcs::io::uring::sqr               *sqr = &service->m_sq.r;
                rcs::io::uring::sqe               *sqe = &sqr->next();

                sqe->opcode     = rcs::io::uring::op::read;
                sqe->descriptor = descriptor;
                sqe->buffer     = buffer;
                sqe->bufsize    = size;
                sqe->offset     = offset;

                token.result       = -1;
                token.continuation = caller;
                sqe->token         = reinterpret_cast<std::uint64_t>(&token);

                sqr->submit();
                service->m_pending.fetch_add(1);
            });
        }
    };

    int ret = co_await awaiter{
        .service    = this,
        .descriptor = descriptor,
        .buffer     = buffer,
        .size       = size,
        .offset     = offset};

    co_return ret;
}

template <rcs::execution::executor TExecutorType>
auto rcs::io::service<TExecutorType>::write(
    std::int32_t descriptor, const void *buffer, std::uint32_t size, std::uint64_t offset)
    -> rcs::co::awaitable<std::int32_t> {
    assert(not busy());

    struct awaiter {
        rcs::io::service<TExecutorType>             *service = nullptr;
        rcs::io::token<int, std::coroutine_handle<>> token   = {-1, nullptr};

        int           descriptor = -1;
        const void   *buffer     = nullptr;
        std::uint32_t bufsize    = 0;
        std::uint64_t offset     = 0;

        auto await_ready() const -> bool { return false; }
        auto await_resume() -> int { return token.result; };

        void await_suspend(std::coroutine_handle<> caller) {
            service->m_executor.execute([caller, this]() {
                const std::unique_lock<std::mutex> sqlock(*service->m_sq.mutex);
                rcs::io::uring::sqr               *sqr = &service->m_sq.r;
                rcs::io::uring::sqe               *sqe = &sqr->next();

                sqe->opcode     = rcs::io::uring::op::write;
                sqe->descriptor = descriptor;
                sqe->buffer     = const_cast<void *>(buffer);
                sqe->bufsize    = bufsize;
                sqe->offset     = offset;

                token.result       = -1;
                token.continuation = caller;
                sqe->token         = reinterpret_cast<std::uint64_t>(&token);

                sqr->submit();
                service->m_pending.fetch_add(1);
            });
        }
    };

    int ret = co_await awaiter{
        .service    = this,
        .descriptor = descriptor,
        .buffer     = buffer,
        .bufsize    = size,
        .offset     = offset};

    co_return ret;
}

template <rcs::execution::executor TExecutorType>
void rcs::io::service<TExecutorType>::run_one() {
    if (idle()) return;

    std::unique_lock<std::mutex> cqlock(*m_cq.mutex);
    rcs::io::uring::cqr         *cqr = &m_cq.r;
    const rcs::io::uring::cqe   *cqe = nullptr;

    if (cqr->empty())
        cqr->wait(1);
    cqe = &cqr->next();

    auto work = [t = cqe->token, result = cqe->result] {
        auto *token = reinterpret_cast<
            rcs::io::token<int, std::coroutine_handle<>> *>(t);

        token->result = result;
        token->continuation.resume();
    };

    cqr->seen();
    m_pending.fetch_sub(1);
    cqlock.unlock();

    m_executor.execute(std::move(work));
}

template <rcs::execution::executor TExecutorType>
void rcs::io::service<TExecutorType>::run() {
    while (not idle()) run_one();
}

// To enable LSP on template functions
template class rcs::io::service<rcs::execution::inline_executor>;

#endif
