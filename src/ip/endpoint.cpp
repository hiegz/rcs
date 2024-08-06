#include <rcs/ip/endpoint.hpp>

#include <cstring>

template <typename TInternetProtocol>
auto rcs::ip::endpoint<TInternetProtocol>::data()
    -> struct ::sockaddr & {
    return reinterpret_cast<
        struct ::sockaddr &>(m_data);
}

template <typename TInternetProtocol>
auto rcs::ip::endpoint<TInternetProtocol>::data() const
    -> const struct ::sockaddr & {
    return reinterpret_cast<
        const struct ::sockaddr &>(m_data);
}

template <typename TInternetProtocol>
auto rcs::ip::endpoint<TInternetProtocol>::operator==(
    const endpoint &other) const -> bool {
    static_assert(sizeof(m_data) == sizeof(other.m_data));
    return 0 == std::memcmp(&m_data, &other.m_data, sizeof(m_data));
}

template <typename TInternetProtocol>
auto rcs::ip::endpoint<TInternetProtocol>::operator!=(
    const endpoint &other) const -> bool {
    static_assert(sizeof(m_data) == sizeof(other.m_data));
    return 0 != std::memcmp(&m_data, &other.m_data, sizeof(m_data));
}

template class rcs::ip::endpoint<rcs::ip::v4>;
template class rcs::ip::endpoint<rcs::ip::v6>;
