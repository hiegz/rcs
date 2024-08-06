#include <rcs/ip/address.hpp>
#include <rcs/ip/v4.hpp>
#include <rcs/ip/v6.hpp>

#include <arpa/inet.h>

#include <stdexcept>
#include <string>

#include <cstring>

template <typename TInternetProtocol>
auto rcs::ip::address<TInternetProtocol>::from_string(const std::string &addrstr)
    -> rcs::ip::address<TInternetProtocol> {
    rcs::ip::address<TInternetProtocol> address = {};
    if (0 >= ::inet_pton(family(), addrstr.data(), &address.m_data))
        throw std::invalid_argument("Invalid address string");
    return address;
}

template <typename TInternetProtocol>
[[nodiscard]] auto rcs::ip::address<TInternetProtocol>::data()
    -> rcs::ip::address<TInternetProtocol>::data_t & { return m_data; }

template <typename TInternetProtocol>
[[nodiscard]] auto rcs::ip::address<TInternetProtocol>::data() const
    -> const rcs::ip::address<TInternetProtocol>::data_t & { return m_data; }

template <typename TInternetProtocol>
[[nodiscard]] auto rcs::ip::address<TInternetProtocol>::operator==(const address &other) const
    -> bool {
    static_assert(sizeof(m_data) == sizeof(other.m_data));
    return 0 == std::memcmp(&m_data, &other.m_data, sizeof(m_data));
}

template <typename TInternetProtocol>
[[nodiscard]] auto rcs::ip::address<TInternetProtocol>::operator!=(const address &other) const
    -> bool {
    static_assert(sizeof(m_data) == sizeof(other.m_data));
    return 0 != std::memcmp(&m_data, &other.m_data, sizeof(m_data));
}

template class rcs::ip::address<rcs::ip::v4>;
template class rcs::ip::address<rcs::ip::v6>;
