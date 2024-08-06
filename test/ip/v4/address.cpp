#include <gtest/gtest.h>

#include <rcs/ip/address.hpp>
#include <rcs/ip/v4.hpp>

#include <stdexcept>
#include <string>

namespace {

TEST(ip_v4_address, any_shouldConstructExpectedAddressString) {
    EXPECT_EQ(rcs::ip::v4::address::any().to_string(), "0.0.0.0");
}

TEST(ip_v4_address, any_shouldConstructUnspecifiedAddress) {
    EXPECT_TRUE(rcs::ip::v4::address::any().is_unspecified());
}

TEST(ip_v4_address, loopback_shouldConstructExpectedAddressString) {
    EXPECT_EQ(rcs::ip::v4::address::loopback().to_string(), "127.0.0.1");
}

TEST(ip_v4_address, loopback_shouldConstructLoopbackAddress) {
    EXPECT_TRUE(rcs::ip::v4::address::loopback().is_loopback());
}

TEST(ip_v4_address, from_string_shouldConstructExpectedAddressString) {
    const std::string addrstr = "59.15.0.0";
    const auto        addr    = rcs::ip::v4::address::from_string(addrstr);

    EXPECT_EQ(
        addr.to_string(),
        addrstr);
}

TEST(ip_v4_address, from_string_shouldThrowOnInvalidAddressString) {
    EXPECT_THROW((void)rcs::ip::v4::address::from_string("59.15.0"),
                 std::invalid_argument);
}

TEST(ip_v4_address, equality_operator_shouldBeEqual) {
    const std::string addrstr = "59.15.0.0";

    const auto left  = rcs::ip::v4::address::from_string(addrstr);
    const auto right = rcs::ip::v4::address::from_string(addrstr);

    EXPECT_EQ(left, right);
}

TEST(ip_v4_address, non_equality_operator_shouldBeNonEqual) {
    const std::string addrstr = "59.15.0.0";

    const auto left  = rcs::ip::v4::address::from_string(addrstr);
    const auto right = rcs::ip::v4::address::loopback();

    EXPECT_NE(left, right);
}

} // namespace
