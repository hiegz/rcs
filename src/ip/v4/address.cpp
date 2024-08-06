#include <rcs/ip/address.hpp>
#include <rcs/ip/v4.hpp> // IWYU pragma: keep

#include <arpa/inet.h>
#include <netinet/in.h>

#include <array>
#include <string>

template <>
auto rcs::ip::address<rcs::ip::v4>::any() noexcept
    -> rcs::ip::address<rcs::ip::v4> {
    rcs::ip::address<rcs::ip::v4> address = {};
    address.m_data.s_addr                 = ::htonl(INADDR_ANY);
    return address;
}

template <>
auto rcs::ip::address<rcs::ip::v4>::loopback() noexcept
    -> rcs::ip::address<rcs::ip::v4> {
    rcs::ip::address<rcs::ip::v4> address = {};
    address.m_data.s_addr                 = ::htonl(INADDR_LOOPBACK);
    return address;
}

template <>
auto rcs::ip::address<rcs::ip::v4>::to_string() const
    -> std::string {
    std::array<char, INET_ADDRSTRLEN> buf = {};
    (void)::inet_ntop(family(), &m_data, buf.data(), buf.size());
    return {buf.begin()};
}

template <>
auto rcs::ip::address<rcs::ip::v4>::is_loopback() const
    -> bool {
    return (m_data.s_addr == ::htonl(INADDR_LOOPBACK));
}

template <>
auto rcs::ip::address<rcs::ip::v4>::is_unspecified() const
    -> bool {
    return (m_data.s_addr == ::htonl(INADDR_ANY));
}
