#ifndef RCS_CO_AWAITABLE_HPP
#define RCS_CO_AWAITABLE_HPP

#include <coroutine> // IWYU pragma: export

#include <exception>
#include <optional>
#include <utility>

#include <cassert>

namespace rcs::co {

/// @brief When returned from final_suspend(), suspends the active coroutine
///        and resumes its caller.
template <typename TPromiseType>
class suspend_forward final {
  public:
    /// @brief Construct from an existing object.
    suspend_forward(const suspend_forward &) = default;

    /// @brief Assign from an existing object.
    auto operator=(const suspend_forward &) -> suspend_forward & = default;

  public:
    /// @brief Default constructor.
    suspend_forward() = default;

    /// @brief Default destructor.
    ~suspend_forward() = default;

  public:
    /// @brief Check if the coroutine should be suspended.
    auto await_ready() const noexcept { return false; }

    /// @brief Suspends the active coroutine and resumes its caller (if any).
    auto await_suspend(std::coroutine_handle<TPromiseType> active)
        const noexcept -> std::coroutine_handle<> {
        TPromiseType &promise = active.promise();
        if (promise.has_caller()) {
            std::coroutine_handle<> caller =
                promise.caller();
            promise.set_caller(nullptr);
            return caller;
        }
        return std::noop_coroutine();
    }

    void await_resume() const noexcept {}
};

template <typename T>
class awaitable;

template <typename T>
class awaitable_frame final {
  public:
    using awaitable_t = rcs::co::awaitable<T>;

  public:
    awaitable_frame(const awaitable_frame &)                     = delete;
    awaitable_frame(awaitable_frame &&)                          = delete;
    auto operator=(const awaitable_frame &) -> awaitable_frame & = delete;
    auto operator=(awaitable_frame &&) -> awaitable_frame      & = delete;

  public:
    /// @brief Default constructor.
    awaitable_frame() = default;

    /// @brief Default destructor.
    ~awaitable_frame() = default;

  public:
    /// @brief Obtain coroutine handle.
    [[nodiscard]] auto get_return_object()
        -> awaitable_frame::awaitable_t {
        const typename awaitable_t::handle_t handle =
            awaitable_t::handle_t::from_promise(*this);
        return awaitable_t{handle};
    }

  public:
    /// @brief Specifies the coroutine's behavior upon its initial invocation.
    [[nodiscard]] auto initial_suspend() const noexcept
        -> std::suspend_never { return {}; }

    /// @brief Specifies the coroutine's behavior when it is about to complete.
    [[nodiscard]] auto final_suspend() const noexcept
        -> rcs::co::suspend_forward<rcs::co::awaitable_frame<T>> {
        return rcs::co::suspend_forward<
            rcs::co::awaitable_frame<T>>{};
    }

  public:
    /// @brief Signal the coroutine's completion with a return value.
    void return_value(const T &value) { m_result = value; }

    /// @brief Signal the coroutine's completion with a return value.
    void return_value(T &&value) { m_result = std::forward<T>(value); }

    /// @brief Capture the exception thrown within the coroutine's body.
    void unhandled_exception() { m_exception = std::current_exception(); }

  public:
    /// @brief Check if the coroutine has completed with a result.
    [[nodiscard]] auto has_result()
        const -> bool { return m_result.has_value(); }

    /// @brief Obtain the result.
    [[nodiscard]] auto result()
        const -> const T & {
        assert(m_result.has_value());
        return m_result.value();
    }

    /// @brief Obtain the result.
    [[nodiscard]] auto result()
        -> T & {
        assert(m_result.has_value());
        return m_result.value();
    }

  public:
    /// @brief Check if the coroutine completed with an exception.
    [[nodiscard]] auto has_exception() const
        -> bool { return m_exception != nullptr; }

    /// @brief Obtain the exception.
    [[nodiscard]] auto exception() const
        -> std::exception_ptr { return m_exception; }

  public:
    /// @brief Check if the coroutine was called by another coroutine.
    [[nodiscard]] auto has_caller() const
        -> bool { return m_caller != nullptr; }

    /// @brief Obtain the caller coroutine.
    [[nodiscard]] auto caller() const
        -> std::coroutine_handle<> { return m_caller; }

    /// @brief Assign the caller coroutine.
    void set_caller(std::coroutine_handle<> caller) {
        m_caller = caller;
    }

  private:
    std::optional<T>        m_result    = std::nullopt;
    std::exception_ptr      m_exception = nullptr;
    std::coroutine_handle<> m_caller    = nullptr;
};

template <typename T>
class awaitable final {
  public:
    using promise_type = rcs::co::awaitable_frame<T>;
    using handle_t     = std::coroutine_handle<rcs::co::awaitable_frame<T>>;

  public:
    awaitable(const awaitable &)                     = delete;
    auto operator=(const awaitable &) -> awaitable & = delete;

  public:
    ///
    /// @brief   Construct from an existing awaitable.
    ///
    /// @details Initializes the underlying coroutine handle with the one
    ///          of `source` whereby `source`'s coroutine handle is reset with
    ///          a null pointer.
    ///
    awaitable(awaitable &&source) noexcept
        : m_handle(std::move(source.m_handle)) {
        source.m_handle = nullptr;
    }

    /// @brief Construct from a coroutine handle.
    explicit awaitable(const awaitable::handle_t &handle)
        : m_handle(handle) {}

    ///
    /// @brief   Assign from an existing awaitable.
    ///
    /// @details Replaces the underlying coroutine with the one of `source`
    ///          whereby `source`'s coroutine handle is reset with a null
    ///          pointer.
    ///
    auto operator=(awaitable &&source) noexcept -> awaitable & {
        m_handle        = source.m_handle;
        source.m_handle = nullptr;
        return *this;
    };

    /// @brief Default destructor.
    ~awaitable() {
        if (m_handle) {
            m_handle.destroy();
        }
    };

  public:
    /// @brief Check if the coroutine has completed.
    [[nodiscard]] auto done() const
        -> bool { return m_handle.done(); }

    /// @brief If coroutine has completed with an exception,
    ///        rethrow that exception.
    void rethrow_exception() const {
        const auto &promise = m_handle.promise();
        if (promise.has_exception())
            std::rethrow_exception(promise.exception());
    }

    /// @brief If coroutine has completed with a result,
    ///        return that result.
    template <typename T_ = T>
        requires(std::is_same_v<T_, T> &&
                 !std::is_void_v<T_>)
    [[nodiscard]] auto result() const
        -> const T_ & {
        awaitable::rethrow_exception();
        assert(m_handle.promise().has_result());
        return m_handle.promise().result();
    }

  public:
    /// @brief Check whether there's any need to suspend.
    template <typename T_ = T>
        requires(std::is_same_v<T_, T> &&
                 !std::is_void_v<T_>)
    [[nodiscard]] auto await_ready() const
        -> bool {
        return m_handle.promise().has_result();
    }

    /// @brief Check whether there's any need to suspend.
    template <typename T_ = T>
        requires(std::is_same_v<T_, T> &&
                 std::is_void_v<T_>)
    [[nodiscard]] auto await_ready() const
        -> bool {
        return m_handle.done();
    }

    /// @brief Suspend until done.
    void await_suspend(std::coroutine_handle<> caller) {
        assert(not m_handle.promise().has_caller());
        m_handle.promise().set_caller(caller);
    }

    /// @brief If coroutine has completed with a result,
    ///        return that result.
    template <typename T_ = T>
        requires(std::is_same_v<T_, T> &&
                 !std::is_void_v<T_>)
    [[nodiscard]] auto await_resume() -> T {
        awaitable::rethrow_exception();
        return std::move(m_handle.promise().result());
    }

    /// @brief If coroutine has completed with an exception,
    ///        rethrow that exception.
    template <typename T_ = T>
        requires(std::is_same_v<T_, T> &&
                 std::is_void_v<T_>)
    void await_resume() const {
        awaitable::rethrow_exception();
    }

  private:
    /// @brief Default constructor.
    awaitable() = default;

    /// @brief Handle to the managed coroutine.
    awaitable::handle_t m_handle = nullptr;
};

} // namespace rcs::co

template <>
class rcs::co::awaitable_frame<void> final {
  public:
    using awaitable_t = rcs::co::awaitable<void>;

  public:
    awaitable_frame(const awaitable_frame &)                     = delete;
    awaitable_frame(awaitable_frame &&)                          = delete;
    auto operator=(const awaitable_frame &) -> awaitable_frame & = delete;
    auto operator=(awaitable_frame &&) -> awaitable_frame      & = delete;

    /// @brief Default destructor.
    ~awaitable_frame() = default;

    /// @brief Default constructor.
    awaitable_frame() = default;

  public:
    /// @brief Obtain coroutine handle.
    [[nodiscard]] auto get_return_object()
        -> awaitable_frame::awaitable_t {
        const typename awaitable_t::handle_t handle =
            awaitable_t::handle_t::from_promise(*this);
        return awaitable_t{handle};
    }

  public:
    /// @brief Specifies the coroutine's behavior upon its initial invocation.
    [[nodiscard]] auto initial_suspend() const
        -> std::suspend_never { return {}; }

    /// @brief Specifies the coroutine's behavior when it is about to complete.
    [[nodiscard]] auto final_suspend() noexcept
        -> rcs::co::suspend_forward<rcs::co::awaitable_frame<void>> {
        return rcs::co::suspend_forward<
            rcs::co::awaitable_frame<void>>{};
    }

  public:
    /// @brief Signal the coroutine's completion with a return value.
    void return_void() {}

    /// @brief Capture the exception thrown within the coroutine's body.
    void unhandled_exception() { m_exception = std::current_exception(); }

  public:
    /// @brief Check if the coroutine completed with an exception.
    [[nodiscard]] auto has_exception() const
        -> bool { return m_exception != nullptr; }

    /// @brief Obtain the exception.
    [[nodiscard]] auto exception() const
        -> std::exception_ptr { return m_exception; }

  public:
    /// @brief Check if the coroutine was called by another coroutine.
    [[nodiscard]] auto has_caller() const
        -> bool { return m_caller != nullptr; }

    /// @brief Obtain the caller coroutine.
    [[nodiscard]] auto caller() const
        -> std::coroutine_handle<> { return m_caller; }

    /// @brief Assign the caller coroutine.
    void set_caller(std::coroutine_handle<> caller) {
        m_caller = caller;
    }

  private:
    std::exception_ptr      m_exception = nullptr;
    std::coroutine_handle<> m_caller    = nullptr;
};

#endif
