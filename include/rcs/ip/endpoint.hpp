#ifndef RCS_IP_ENDPOINT_HPP
#define RCS_IP_ENDPOINT_HPP

#include <netinet/in.h>

#include <cstdint>

struct sockaddr;

namespace rcs::ip {

class v4;
class v6;

template <typename TInternetProtocol>
class address;

template <typename TInternetProtocol>
struct endpoint_data;

template <>
struct endpoint_data<rcs::ip::v4> {
    using type = struct ::sockaddr_in;
};

template <>
struct endpoint_data<rcs::ip::v6> {
    using type = struct ::sockaddr_in6;
};

/// @brief Version-independent IP endpoint interface.
template <typename TInternetProtocol>
class endpoint final {
  public:
    /// @brief Endpoint data type.
    using data_t = rcs::ip::endpoint_data<TInternetProtocol>::type;

    /// @brief Internet protocol version.
    using ip_v = TInternetProtocol;

  public:
    /// @brief Construct from an existing endpoint.
    endpoint(const endpoint &) = default;

    /// @brief Assign from an existing endpoint.
    auto operator=(const endpoint &)
        -> endpoint & = default;

  public:
    /// @brief Default destructor.
    ~endpoint() = default;

    /// @brief Default constructor.
    endpoint();

    /// @brief Construct from an address and a port value.
    explicit endpoint(const rcs::ip::address<TInternetProtocol> &addr, std::uint16_t port = 0);

    /// @brief Construct from a port value.
    explicit endpoint(std::uint16_t port);

  public:
    /// @brief Get the endpoint size.
    [[nodiscard]] static constexpr auto size()
        -> std::uint32_t { return sizeof(m_data); }

  public:
    /// @brief Set the endpoint address.
    void set_address(const rcs::ip::address<TInternetProtocol> &address);

    /// @brief Set the endpoint port.
    void set_port(std::uint16_t port);

  public:
    /// @brief Get the endpoint address.
    [[nodiscard]] auto address() const
        -> const rcs::ip::address<endpoint::ip_v> &;

    /// @brief Get the endpoint port.
    [[nodiscard]] auto port() const
        -> std::uint16_t;

    /// @brief Get the endpoint data.
    [[nodiscard]] auto data()
        -> struct ::sockaddr &;

    /// @brief Get the endpoint data.
    [[nodiscard]] auto data() const
        -> const struct ::sockaddr &;

  public:
    /// @brief Compare two endpoints for equality.
    [[nodiscard]] auto operator==(const endpoint &other) const
        -> bool;

    /// @brief Compare two endpoints for inequality.
    [[nodiscard]] auto operator!=(const endpoint &other) const
        -> bool;

  private:
    /// @brief IP endpoint data.
    rcs::ip::endpoint<TInternetProtocol>::data_t m_data = {};
};

} // namespace rcs::ip

#endif
