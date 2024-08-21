#ifndef RCS_HEX_HPP
#define RCS_HEX_HPP

#include <rcs/lc.hpp>

#include <cstdint>

namespace rcs::hex {

/// @brief Encode a nibble to its hexadecimal equivalent.
template <rcs::lc TLetterCase = rcs::lc::lowercase>
auto encode(std::uint8_t in) -> std::int8_t;

/// @brief Encode an array of bytes to an array of hexadecimal ASCII characters.
template <rcs::lc TLetterCase = rcs::lc::lowercase>
void encode(const void *in, std::uint32_t inlen, char *out);

/// @brief Decode a single case-insensitive hexadecimal ASCII character to its decimal equivalent.
auto decode(std::int8_t in) -> std::uint8_t;

/// @brief Decode an array of hexadecimal characters to an array of bytes.
void decode(const char *in, std::uint32_t inlen, void *out);

} // namespace rcs::hex

#endif
