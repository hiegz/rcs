#include <rcs/hex.hpp>
#include <rcs/lc.hpp>

#include <cstdint>

/* * */

template <>
auto rcs::hex::encode<rcs::lc::lowercase>(std::uint8_t in) -> std::int8_t {
    constexpr const char *lut = "0123456789abcdef";
    return lut[in & 0xf];
}

template <>
auto rcs::hex::encode<rcs::lc::uppercase>(std::uint8_t in) -> std::int8_t {
    constexpr const char *lut = "0123456789ABCDEF";
    return lut[in & 0xf];
}

template <rcs::lc TLetterCase>
void rcs::hex::encode(const void *in, std::uint32_t inlen, char *out) {
    const std::uint8_t *bin = reinterpret_cast<const std::uint8_t *>(in);

    std::uint32_t i;
    std::uint32_t j;

    for (i = 0, j = 0; i < inlen; ++i, j += 2) {
        out[j + 0] = rcs::hex::encode<TLetterCase>(bin[i] >> 4);
        out[j + 1] = rcs::hex::encode<TLetterCase>(bin[i] >> 0);
    }
}

/* * */

template void rcs::hex::encode<rcs::lc::lowercase>(const void *in, std::uint32_t inlen, char *out);
template void rcs::hex::encode<rcs::lc::uppercase>(const void *in, std::uint32_t inlen, char *out);

/* * */

auto rcs::hex::decode(std::int8_t in) -> std::uint8_t {
    return ((in & 0xf) + (in >> 6) * 9);
}

void rcs::hex::decode(const char *in, std::uint32_t inlen, void *out) {
    std::uint8_t *bout = reinterpret_cast<std::uint8_t *>(out);

    for (std::uint32_t i = 0, j = 0; j + 1 < inlen; ++i, j += 2) {
        bout[i] = (rcs::hex::decode(in[j + 0]) << 4) | rcs::hex::decode(in[j + 1]);
    }
}
