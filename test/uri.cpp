#include <gtest/gtest.h>

#include <rcs/lc.hpp>
#include <rcs/uri.hpp>

#include <string>

/* * */

class rcs_uri : public ::testing::Test {};

/* * */

TEST_F(rcs_uri, lowercase_encode_shouldPercentEncodeGivenString) {
    const std::string expected = "http%3a%2f%2flocalhost%3a5915%2fdrive%2ffolder+with+spaces%2ffile+with++spaces.pdf";
    const std::string in       = "http://localhost:5915/drive/folder with spaces/file with  spaces.pdf";
    const std::string out      = rcs::uri::encode<rcs::lc::lowercase>(in);

    EXPECT_EQ(out, expected);
}

TEST_F(rcs_uri, uppercase_encode_shouldPercentEncodeGivenString) {
    const std::string expected = "http%3A%2F%2Flocalhost%3A5915%2Fdrive%2Ffolder+with+spaces%2Ffile+with++spaces.pdf";
    const std::string in       = "http://localhost:5915/drive/folder with spaces/file with  spaces.pdf";
    const std::string out      = rcs::uri::encode<rcs::lc::uppercase>(in);

    EXPECT_EQ(out, expected);
}

TEST_F(rcs_uri, lowercase_decode_shouldPercentEncodeGivenString) {
    const std::string expected = "http://localhost:5915/drive/folder with spaces/file with  spaces.pdf";
    const std::string in       = "http%3a%2f%2flocalhost%3a5915%2fdrive%2ffolder+with+spaces%2ffile+with++spaces.pdf";
    const std::string out      = rcs::uri::decode(in);

    EXPECT_EQ(out, expected);
}

TEST_F(rcs_uri, uppercase_decode_shouldPercentEncodeGivenString) {
    const std::string expected = "http://localhost:5915/drive/folder with spaces/file with  spaces.pdf";
    const std::string in       = "http%3A%2F%2Flocalhost%3A5915%2Fdrive%2Ffolder+with+spaces%2Ffile+with++spaces.pdf";
    const std::string out      = rcs::uri::decode(in);

    EXPECT_EQ(out, expected);
}
