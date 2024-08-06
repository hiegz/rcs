#include <rcs/ip/address.hpp>
#include <rcs/ip/v6.hpp> // IWYU pragma: keep

#include <arpa/inet.h>
#include <netinet/in.h>

#include <array>
#include <string>

#include <cstring>

template <>
auto rcs::ip::address<rcs::ip::v6>::any() noexcept
    -> rcs::ip::address<rcs::ip::v6> {
    rcs::ip::address<rcs::ip::v6> address = {};
    address.m_data                        = IN6ADDR_ANY_INIT;
    return address;
}

template <>
auto rcs::ip::address<rcs::ip::v6>::loopback() noexcept
    -> rcs::ip::address<rcs::ip::v6> {
    rcs::ip::address<rcs::ip::v6> address = {};
    address.m_data                        = IN6ADDR_LOOPBACK_INIT;
    return address;
}

template <>
auto rcs::ip::address<rcs::ip::v6>::to_string() const
    -> std::string {
    std::array<char, INET6_ADDRSTRLEN> buf = {};
    (void)::inet_ntop(family(), &m_data, buf.data(), buf.size());
    return {buf.begin()};
}

template <>
auto rcs::ip::address<rcs::ip::v6>::is_loopback() const
    -> bool {
    static_assert(sizeof(m_data) == sizeof(in6addr_loopback));
    return (0 == std::memcmp(&m_data, &in6addr_loopback, sizeof(m_data)));
}

template <>
auto rcs::ip::address<rcs::ip::v6>::is_unspecified() const
    -> bool {
    static_assert(sizeof(m_data) == sizeof(in6addr_any));
    return (0 == std::memcmp(&m_data, &in6addr_any, sizeof(m_data)));
}
