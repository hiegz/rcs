#include <rcs/ip/address.hpp>
#include <rcs/ip/endpoint.hpp>
#include <rcs/ip/v4.hpp>

#include <netinet/in.h>

#include <cstdint>

template <>
void rcs::ip::endpoint<rcs::ip::v4>::set_address(
    const rcs::ip::address<rcs::ip::v4> &address) {
    m_data.sin_addr = address.data();
}

template <>
void rcs::ip::endpoint<rcs::ip::v4>::set_port(
    std::uint16_t port) {
    m_data.sin_port = ::htons(port);
}

template <>
rcs::ip::endpoint<rcs::ip::v4>::endpoint() {
    m_data.sin_family = rcs::ip::address<rcs::ip::v4>::family();
}

template <>
rcs::ip::endpoint<rcs::ip::v4>::endpoint(
    const rcs::ip::address<rcs::ip::v4> &address, std::uint16_t port)
    : rcs::ip::endpoint<rcs::ip::v4>() {
    set_address(address);
    set_port(port);
}

template <>
rcs::ip::endpoint<rcs::ip::v4>::endpoint(
    std::uint16_t port)
    : rcs::ip::endpoint<rcs::ip::v4>() {
    set_port(port);
}

template <>
auto rcs::ip::endpoint<rcs::ip::v4>::address() const
    -> const rcs::ip::address<rcs::ip::v4> & {
    return reinterpret_cast<
        const rcs::ip::address<rcs::ip::v4> &>(m_data.sin_addr);
}

template <>
auto rcs::ip::endpoint<rcs::ip::v4>::port() const
    -> std::uint16_t {
    return ::ntohs(m_data.sin_port);
}
