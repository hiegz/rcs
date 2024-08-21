#include <gtest/gtest.h>

#include <rcs/hex.hpp>
#include <rcs/lc.hpp>

#include <array>
#include <string>

#include <cstdint>

/* * */

class rcs_hex : public ::testing::Test {};

/* * */

TEST_F(rcs_hex, lowercase_encode_shouldEncodeGivenBytes) {
    const std::string                 expected = "0120dfc32e";
    const std::array<std::uint8_t, 5> in       = {0x01, 0x20, 0xdf, 0xc3, 0x2e};
    std::string                       out      = {};
    out.resize(in.size() * 2);

    rcs::hex::encode<rcs::lc::lowercase>(in.data(), in.size(), out.data());

    EXPECT_EQ(out, expected);
}

TEST_F(rcs_hex, uppercase_encode_shouldEncodeGivenBytes) {
    const std::string                 expected = "0120DFC32E";
    const std::array<std::uint8_t, 5> in       = {0x01, 0x20, 0xdf, 0xc3, 0x2e};
    std::string                       out      = {};
    out.resize(in.size() * 2);

    rcs::hex::encode<rcs::lc::uppercase>(in.data(), in.size(), out.data());

    EXPECT_EQ(out, expected);
}

TEST_F(rcs_hex, lowercase_decode_shouldDecodeGivenString) {
    const std::array<std::uint8_t, 5> expected = {0x5d, 0xc4, 0x0f, 0xc9, 0xcc};
    const std::string                 in       = "5dc40fc9cc";
    std::array<std::uint8_t, 5>       out      = {};

    rcs::hex::decode(in.data(), in.size(), out.data());

    EXPECT_EQ(out, expected);
}

TEST_F(rcs_hex, uppercase_decode_shouldDecodeGivenString) {
    const std::array<std::uint8_t, 5> expected = {0x5d, 0xc4, 0x0f, 0xc9, 0xcc};
    const std::string                 in       = "5DC40FC9CC";
    std::array<std::uint8_t, 5>       out      = {};

    rcs::hex::decode(in.data(), in.size(), out.data());

    EXPECT_EQ(out, expected);
}
