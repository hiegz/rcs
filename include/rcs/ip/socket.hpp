#ifndef RCS_IP_SOCKET_HPP
#define RCS_IP_SOCKET_HPP

#include <rcs/co/awaitable.hpp>

#include <rcs/execution/executor.hpp>
#include <rcs/execution/inline_executor.hpp>

#include <rcs/system/exception.hpp>
#include <rcs/system/handle.hpp>

#include <rcs/ip/address.hpp>
#include <rcs/ip/endpoint.hpp>
#include <rcs/ip/tcp.hpp>
#include <rcs/ip/udp.hpp>
#include <rcs/ip/v4.hpp>
#include <rcs/ip/v6.hpp>

#include <rcs/io/service.hpp>

#include <sys/socket.h>

#include <exception>

#include <cerrno>
#include <cstdint>

namespace rcs::ip {

template <typename TCommunicationProtocol, rcs::execution::executor TExecutorType>
class socket final {
  public:
    /// @brief Internet protocol version.
    using ip_v = TCommunicationProtocol::ip_t;

    /// @brief Communication protocol.
    using cp_t = TCommunicationProtocol;

    /// @brief Address type.
    using address_t = rcs::ip::address<ip_v>;

    /// @brief Endpoint type.
    using endpoint_t = rcs::ip::endpoint<ip_v>;

    /// @brief Executor type.
    using executor_t = TExecutorType;

    /// @brief Service type.
    using service_t = rcs::io::service<socket::executor_t>;

  public:
    /// @brief Get the protocol address family
    [[nodiscard]] static constexpr auto family() noexcept
        -> decltype(TCommunicationProtocol::family()) { return TCommunicationProtocol::family(); }

    /// @brief Get the protocol type.
    [[nodiscard]] static constexpr auto type() noexcept
        -> int { return TCommunicationProtocol::type(); }

  public:
    socket(const socket &)                     = delete;
    auto operator=(const socket &) -> socket & = delete;

  public:
    /// @brief Construct from an existing socket.
    socket(socket &&) = default;

    /// @brief Assign from an existing socket.
    auto operator=(socket &&) -> socket & = default;

    /// @brief Consturct a closed socket.
    explicit socket(rcs::io::service<TExecutorType> &service)
        : m_service(&service) {};

    /// @brief Consturct a closed socket.
    socket(TCommunicationProtocol /* protocol */, rcs::io::service<TExecutorType> &service)
        : socket(service) {};

    /// @brief Destroy the resources utilized by the socket.
    ~socket() = default;

  public:
    /// @brief Get the socket identifier.
    auto descriptor() const
        -> std::int32_t { return m_handle.descriptor(); }

    /// @brief Get the socket endpoint.
    auto endpoint() const
        -> const socket::endpoint_t & { return m_endpoint; }

  public:
    /// @brief Open the socket on the file desciprtor issued by the kernel.
    void open() {
        std::int32_t rv;
        rv = ::socket(socket::family(), socket::type(), 0);
        if (-1 == rv) throw rcs::system::exception(errno);
        m_handle = rv;
    };

    /// @brief Close the socket.
    void close() {
        if (-1 == m_handle.descriptor()) return;
        m_handle.reset();
    }

  public:
    /// @brief Bind the socket to an IP endpoint.
    void bind(const socket::endpoint_t &endpoint) {
        std::int32_t rv;
        rv = ::bind(m_handle.descriptor(),
                    &endpoint.data(),
                    endpoint.size());
        if (-1 == rv) throw rcs::system::exception(errno);
        m_endpoint = endpoint;
    }

    /// @brief Listen for incoming connections.
    void listen(int bandwidth) {
        std::int32_t rv;
        rv = ::listen(m_handle.descriptor(), bandwidth);
        if (-1 == rv) throw rcs::system::exception(errno);
    }

  public:
    /// @brief Accept a connection on a socket.
    auto accept() -> rcs::co::awaitable<socket> {
        std::int32_t rv;
        socket       connection(*m_service);

        rv = co_await m_service->accept(
            m_handle.descriptor(),
            connection.m_endpoint.data(),
            connection.m_endpoint.size());

        if (rv < 0) throw rcs::system::exception(-rv);
        connection.m_handle = rv;

        co_return connection;
    }

  public:
    /// @brief Read at most the specified number of bytes from a remote endpoint.
    auto recv(void *buf, std::uint32_t bufsize)
        -> rcs::co::awaitable<std::uint32_t> {
        std::int32_t rv = co_await m_service->read(m_handle.descriptor(), buf, bufsize, 0);
        if (rv < 0) throw rcs::system::exception(rv);
        co_return rv;
    }

    /// @brief Read the specified number of bytes from a remote endpoint.
    auto recvall(void *buf, std::uint32_t *bufsize)
        -> rcs::co::awaitable<void> {
        std::int32_t  rv    = 0;
        std::uint32_t total = *bufsize;
        std::uint32_t read  = 0;
        while (read < total) {
            try {
                rv = co_await socket::recv(
                    reinterpret_cast<std::uint8_t *>(buf) + read, total - read);
                if (0 == rv) break;
                read += rv;
            } catch (std::exception &e) {
                *bufsize = read;
                throw e;
            }
        }

        *bufsize = read;
    }

    /// @brief Send at most the specified number of bytes to a remote endpoint.
    auto send(const void *buf, std::uint32_t bufsize)
        -> rcs::co::awaitable<std::uint32_t> {
        std::int32_t rv = co_await m_service->write(m_handle.descriptor(), buf, bufsize, 0);
        if (rv < 0) throw rcs::system::exception(rv);
        co_return rv;
    }

    /// @brief Send the specified number of bytes to a remote endpoint.
    auto sendall(const void *buf, std::uint32_t *bufsize)
        -> rcs::co::awaitable<void> {
        std::int32_t  rv    = 0;
        std::uint32_t total = *bufsize;
        std::uint32_t sent  = 0;
        while (sent < total) {
            try {
                rv = co_await socket::send(
                    reinterpret_cast<const std::uint8_t *>(buf) + sent, total - sent);
                if (0 == rv) break;
                sent += rv;
            } catch (std::exception &e) {
                *bufsize = sent;
                throw e;
            }
        }

        *bufsize = sent;
    }

  private:
    /// @brief Default constructor.
    socket() = default;

  private:
    /// @brief I/O service
    socket::service_t *m_service = nullptr;

    /// @brief ...
    rcs::system::handle m_handle = -1;

    /// @brief ...
    socket::endpoint_t m_endpoint;
};

} // namespace rcs::ip

template class rcs::ip::socket<rcs::ip::v4::tcp, rcs::execution::inline_executor>;
template class rcs::ip::socket<rcs::ip::v4::udp, rcs::execution::inline_executor>;
template class rcs::ip::socket<rcs::ip::v6::tcp, rcs::execution::inline_executor>;
template class rcs::ip::socket<rcs::ip::v6::udp, rcs::execution::inline_executor>;

#endif
