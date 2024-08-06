#ifndef RCS_IP_TCP_HPP
#define RCS_IP_TCP_HPP

#include <sys/socket.h> // IWYU pragma: keep

namespace rcs::ip {

/// @brief Encapsulates the flags identifying TCP/IP.
template <typename TInternetProtocol>
class tcp final {
  public:
    tcp(const tcp &)                     = default;
    tcp(tcp &&)                          = default;
    auto operator=(const tcp &) -> tcp & = default;
    auto operator=(tcp &&) -> tcp      & = default;

  public:
    /// @brief Internet protocol type.
    using ip_t = TInternetProtocol;

  public:
    /// @brief Get the protocol address family
    [[nodiscard]] static constexpr auto family() noexcept
        -> decltype(TInternetProtocol::family()) { return TInternetProtocol::family(); }

    /// @brief Get the protocol type.
    [[nodiscard]] static constexpr auto type() noexcept
        -> int { return SOCK_STREAM; }

  public:
    /// @brief Default constructor.
    tcp() = default;

    /// @brief Default destructor.
    ~tcp() = default;
};

} // namespace rcs::ip

#endif
