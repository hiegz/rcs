#ifndef RCS_IP_V6_HPP
#define RCS_IP_V6_HPP

#include <sys/socket.h>

namespace rcs::ip {

template <typename TInternetProtocol>
class tcp;

template <typename TInternetProtocol>
class udp;

/// @brief Encapsulates the flags identifying IPv6.
class v6 final {
  public:
    v6(const v6 &)                     = default;
    v6(v6 &&)                          = default;
    auto operator=(const v6 &) -> v6 & = default;
    auto operator=(v6 &&) -> v6      & = default;

  public:
    /// @brief Get the protocol address family.
    [[nodiscard]] static constexpr auto family() noexcept
        -> int { return AF_INET6; }

  public:
    /// @brief TCP/IP version 6.
    using tcp = rcs::ip::tcp<v6>;

    /// @brief UDP/IP version 6.
    using udp = rcs::ip::udp<v6>;

  public:
    /// @brief Default constructor.
    v6() = default;

    /// @brief Default destructor.
    ~v6() = default;
};

} // namespace rcs::ip

#endif
