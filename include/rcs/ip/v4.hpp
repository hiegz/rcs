#ifndef RCS_IP_V4_HPP
#define RCS_IP_V4_HPP

#include <sys/socket.h>

namespace rcs::ip {

template <typename TInternetProtocol>
class tcp;

template <typename TInternetProtocol>
class udp;

template <typename TInternetProtocol>
class address;

template <typename TInternetProtocol>
class endpoint;

/// @brief Encapsulates the flags identifying IPv4.
class v4 final {
  public:
    v4(const v4 &)                     = default;
    v4(v4 &&)                          = default;
    auto operator=(const v4 &) -> v4 & = default;
    auto operator=(v4 &&) -> v4      & = default;

  public:
    /// @brief Get the protocol address family.
    [[nodiscard]] static constexpr auto family() noexcept
        -> int { return AF_INET; }

  public:
    /// @brief TCP/IP version 4.
    using tcp = rcs::ip::tcp<v4>;

    /// @brief UDP/IP version 4.
    using udp = rcs::ip::udp<v4>;

  public:
    /// @brief IP version 4 address.
    using address = rcs::ip::address<v4>;

    /// @brief IP version 4 endpoint.
    using endpoint = rcs::ip::endpoint<v4>;

  public:
    /// @brief Default constructor.
    v4() = default;

    /// @brief Default destructor.
    ~v4() = default;
};

} // namespace rcs::ip

#endif
