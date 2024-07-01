#ifndef RCS_SYSTEM_EXCEPTION_HPP
#define RCS_SYSTEM_EXCEPTION_HPP

#include <exception>

#include <cstdint>
#include <cstring>

namespace rcs::system {

/// @brief Defines a type of object to be thrown as exception. It reports
///        errors originating from the operating system or other low-level
///        application interfaces.
class exception final : public std::exception {
  public:
    /// @brief Construct from an error code.
    explicit exception(std::int32_t ec)
        : m_ec(ec) {}

    /// @brief Get the error code.
    [[nodiscard]] auto error_code() const
        -> std::int32_t { return m_ec; }

    /// @brief Get an explanatory string.
    [[nodiscard]] auto what() const noexcept
        -> const char * override {
        return ::strerror(m_ec);
    }

  private:
    /// @brief Default constructor.
    exception() = default;

    /// @brief Error code.
    std::int32_t m_ec = 0;
};

} // namespace rcs::system

#endif
