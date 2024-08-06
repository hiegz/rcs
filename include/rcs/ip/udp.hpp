#ifndef RCS_IP_UDP_HPP
#define RCS_IP_UDP_HPP

#include <sys/socket.h> // IWYU pragma: keep

namespace rcs::ip {

/// @brief Encapsulates the flags identifying UDP/IP.
template <typename TInternetProtocol>
class udp final {
  public:
    udp(const udp &)                     = default;
    udp(udp &&)                          = default;
    auto operator=(const udp &) -> udp & = default;
    auto operator=(udp &&) -> udp      & = default;

  public:
    /// @brief Internet protocol type.
    using ip_t = TInternetProtocol;

  public:
    /// @brief Get the protocol address family
    [[nodiscard]] static constexpr auto family() noexcept
        -> decltype(TInternetProtocol::family()) { return TInternetProtocol::family(); }

    /// @brief Get the protocol type.
    [[nodiscard]] static constexpr auto type() noexcept
        -> int { return SOCK_DGRAM; }

  public:
    /// @brief Default constructor.
    udp() = default;

    /// @brief Default destructor.
    ~udp() = default;
};

} // namespace rcs::ip

#endif
