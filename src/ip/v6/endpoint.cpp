#include <rcs/ip/address.hpp>
#include <rcs/ip/endpoint.hpp>
#include <rcs/ip/v6.hpp>

#include <netinet/in.h>

#include <cstdint>

template <>
void rcs::ip::endpoint<rcs::ip::v6>::set_address(
    const rcs::ip::address<rcs::ip::v6> &address) {
    m_data.sin6_addr = address.data();
}

template <>
void rcs::ip::endpoint<rcs::ip::v6>::set_port(
    std::uint16_t port) {
    m_data.sin6_port = ::htons(port);
}

template <>
rcs::ip::endpoint<rcs::ip::v6>::endpoint() {
    m_data.sin6_family = rcs::ip::address<rcs::ip::v6>::family();
}

template <>
rcs::ip::endpoint<rcs::ip::v6>::endpoint(
    const rcs::ip::address<rcs::ip::v6> &address, std::uint16_t port)
    : rcs::ip::endpoint<rcs::ip::v6>() {
    set_address(address);
    set_port(port);
}

template <>
rcs::ip::endpoint<rcs::ip::v6>::endpoint(
    std::uint16_t port)
    : rcs::ip::endpoint<rcs::ip::v6>() {
    set_port(port);
}

template <>
auto rcs::ip::endpoint<rcs::ip::v6>::address() const
    -> const rcs::ip::address<rcs::ip::v6> & {
    return reinterpret_cast<
        const rcs::ip::address<rcs::ip::v6> &>(m_data.sin6_addr);
}

template <>
auto rcs::ip::endpoint<rcs::ip::v6>::port() const
    -> std::uint16_t {
    return ::ntohs(m_data.sin6_port);
}
