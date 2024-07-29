#ifndef RCS_EXECUTION_INLINE_EXECUTOR_HPP
#define RCS_EXECUTION_INLINE_EXECUTOR_HPP

#include <functional>
#include <utility>

namespace rcs::execution {

///
/// @brief   Inline executor.
///
/// @details Implements an executor that ensures immediate execution of an
///          invokable task such that it is functionally equivalent
///          to invoking the task directly.
///
class inline_executor final {
  public:
    inline_executor(const inline_executor &) noexcept                     = default;
    inline_executor(inline_executor &&) noexcept                          = default;
    auto operator=(const inline_executor &) noexcept -> inline_executor & = default;
    auto operator=(inline_executor &&) noexcept -> inline_executor      & = default;

    /// @brief Default destructor.
    ~inline_executor() = default;

    /// @brief Default constructor.
    inline_executor() = default;

  public:
    /// @brief Execute a unit of work.
    template <typename TWork>
    void execute(TWork &&work) {
        std::invoke(std::forward<TWork>(work));
    }

  public:
    /// @brief Compare two inline executors.
    constexpr auto operator==(
        const inline_executor &other) const
        -> bool {
        (void)other;
        return true;
    }
};

} // namespace rcs::execution

#endif
