#ifndef RCS_IP_ADDRESS_HPP
#define RCS_IP_ADDRESS_HPP

#include <netinet/in.h>

#include <cstdint>
#include <string>

namespace rcs::ip {

class v4;
class v6;

template <typename UInternetProtocol>
struct address_data;

template <>
struct address_data<rcs::ip::v4> {
    using type = struct ::in_addr;
};

template <>
struct address_data<rcs::ip::v6> {
    using type = struct ::in6_addr;
};

/// @brief Version-independent IP address interface.
template <typename TInternetProtocol>
class address final {
  public:
    /// @brief Address data type.
    using data_t = rcs::ip::address_data<TInternetProtocol>::type;

    /// @brief Internet protocol version.
    using ip_v = TInternetProtocol;

  public:
    /// @brief Construct from an existing address.
    address(const address &) = default;

    /// @brief Assign from an existing address.
    auto operator=(const address &)
        -> address & = default;

  public:
    /// @brief Default destructor.
    ~address() = default;

    /// @brief Default constructor.
    address() = default;

  public:
    /// @brief Get the address family.
    [[nodiscard]] static constexpr auto family() noexcept
        -> decltype(address::ip_v::family()) { return address::ip_v::family(); }

    /// @brief Get the address size.
    [[nodiscard]] static constexpr auto size()
        -> std::uint32_t { return sizeof(m_data); }

  public:
    /// @brief Construct an unspecified address.
    [[nodiscard]] static auto any() noexcept
        -> address;

    /// @brief Construct a loopback address.
    [[nodiscard]] static auto loopback() noexcept
        -> address;

    /// @brief Construct an internet address from its string representation.
    [[nodiscard]] static auto from_string(const std::string &addrstr)
        -> address;

  public:
    /// @brief Get the address data.
    [[nodiscard]] auto data()
        -> rcs::ip::address<TInternetProtocol>::data_t &;

    /// @brief Get the address data.
    [[nodiscard]] auto data() const
        -> const rcs::ip::address<TInternetProtocol>::data_t &;

  public:
    /// @brief Get the address string.
    [[nodiscard]] auto to_string() const -> std::string;

    /// @brief Check whether the address is a loopback address.
    [[nodiscard]] auto is_loopback() const -> bool;

    /// @brief Check whether the address is unspecified.
    [[nodiscard]] auto is_unspecified() const -> bool;

  public:
    /// @brief Compare two addresses for equality.
    [[nodiscard]] auto operator==(const address &other) const
        -> bool;

    /// @brief Compare two addresses for inequality.
    [[nodiscard]] auto operator!=(const address &other) const
        -> bool;

  private:
    /// @brief Information about the IP address.
    rcs::ip::address<TInternetProtocol>::data_t m_data = {};
};

} // namespace rcs::ip

#endif
