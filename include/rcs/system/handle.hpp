#ifndef RCS_SYSTEM_HANDLE_HPP
#define RCS_SYSTEM_HANDLE_HPP

#include <cstdint>

namespace rcs::system {

/// @brief Container object managing a process-unique identifier for a file or
///        other input output resource, such as a pipe or network socket.
class handle final {
  public:
    handle(const handle &)                     = delete;
    auto operator=(const handle &) -> handle & = delete;

  public:
    using type = std::int32_t;

  public:
    /// @brief Construct from an existing handle.
    handle(handle &&other) noexcept;

    /// @brief Assign from an existing handle.
    auto operator=(handle &&other) noexcept -> handle &;

    /// @brief Construct from an unmanaged resource identifier.
    handle(handle::type descriptor = -1); // NOLINT

    /// @brief Destroy managed resource identifier.
    ~handle();

  public:
    /// @brief Obtain managed resource identifier.
    auto descriptor() const -> const handle::type &;

  public:
    /// @brief Release ownership over a resource identifier.
    auto release() -> handle::type;

    /// @brief   Reset the container.
    void reset(handle::type descriptor = -1);

  private:
    /// @brief Managed resource identifier.
    handle::type m_descriptor = -1;
};

} // namespace rcs::system

#endif
