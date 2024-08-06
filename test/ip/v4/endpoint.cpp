#include <gtest/gtest.h>

#include <rcs/ip/address.hpp>
#include <rcs/ip/endpoint.hpp>
#include <rcs/ip/v4.hpp>

#include <cstdint>

namespace {

TEST(ip_v4_endpoint, constructor_shouldConstructExpectedAddress) {
    const auto address  = rcs::ip::v4::address::loopback();
    const auto endpoint = rcs::ip::v4::endpoint(address);

    EXPECT_EQ(address, endpoint.address());
}

TEST(ip_v4_endpoint, constructor_shouldConstructExpectedPort) {
    const std::uint16_t port     = 5915;
    const auto          endpoint = rcs::ip::v4::endpoint(port);

    EXPECT_EQ(port, endpoint.port());
}

TEST(ip_v4_endpoint, equality_operator_shouldBeEqual) {
    const auto left  = rcs::ip::v4::endpoint(rcs::ip::v4::address::loopback(), 5915);
    const auto right = rcs::ip::v4::endpoint(rcs::ip::v4::address::loopback(), 5915);

    EXPECT_EQ(left, right);
}

TEST(ip_v4_endpoint, nonequality_operator_shouldNotBeEqual) {
    const auto left  = rcs::ip::v4::endpoint(rcs::ip::v4::address::any(), 5916);
    const auto right = rcs::ip::v4::endpoint(rcs::ip::v4::address::loopback(), 5915);

    EXPECT_NE(left, right);
}

} // namespace
