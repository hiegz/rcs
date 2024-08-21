#include <gtest/gtest.h>

#include <rcs/lc.hpp>
#include <rcs/uri.hpp>

#include <format>
#include <map>
#include <ostream>
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

/* * */

struct rcs_uri_parse_scheme_param {
    std::string in;
    std::string out;

    friend auto operator<<(std::ostream &out, const struct rcs_uri_parse_scheme_param &param) -> std::ostream & {
        return (out << std::format("{{in: '{}' | out: '{}'}}", param.in, param.out));
    }
};

class rcs_uri_parse_scheme : public ::testing::TestWithParam<struct rcs_uri_parse_scheme_param> {};

TEST_P(rcs_uri_parse_scheme, shouldParseScheme) {
    EXPECT_EQ(rcs::uri::parse(GetParam().in).scheme,
              GetParam().out);
}

INSTANTIATE_TEST_CASE_P(
    ,
    rcs_uri_parse_scheme,
    ::testing::Values(
        rcs_uri_parse_scheme_param("", ""),
        rcs_uri_parse_scheme_param("://", ""),
        rcs_uri_parse_scheme_param("s://", "s"),
        rcs_uri_parse_scheme_param("sc://", "sc"),
        rcs_uri_parse_scheme_param("scheme://", "scheme")));

/* * */

struct rcs_uri_parse_host_param {
    std::string in;
    std::string out;

    friend auto operator<<(std::ostream &out, const struct rcs_uri_parse_host_param &param) -> std::ostream & {
        return (out << std::format("{{in: '{}' | out: '{}'}}", param.in, param.out));
    }
};

class rcs_uri_parse_host : public ::testing::TestWithParam<struct rcs_uri_parse_host_param> {};

TEST_P(rcs_uri_parse_host, shouldParseHost) {
    EXPECT_EQ(rcs::uri::parse(GetParam().in).host,
              GetParam().out);
}

INSTANTIATE_TEST_CASE_P(
    ,
    rcs_uri_parse_host,
    ::testing::Values(
        rcs_uri_parse_host_param("", ""),
        rcs_uri_parse_host_param("://", ""),
        rcs_uri_parse_host_param(":///", ""),
        rcs_uri_parse_host_param("://?", ""),
        rcs_uri_parse_host_param("://#", ""),
        rcs_uri_parse_host_param("host", "host"),
        rcs_uri_parse_host_param("://host", "host"),
        rcs_uri_parse_host_param("://host/", "host"),
        rcs_uri_parse_host_param("://host?", "host"),
        rcs_uri_parse_host_param("://host#", "host")));

/* * */

struct rcs_uri_parse_path_param {
    std::string in;
    std::string out;

    friend auto operator<<(std::ostream &out, const struct rcs_uri_parse_path_param &param) -> std::ostream & {
        return (out << std::format("{{in: '{}' | out: '{}'}}", param.in, param.out));
    }
};

class rcs_uri_parse_path : public ::testing::TestWithParam<struct rcs_uri_parse_path_param> {};

TEST_P(rcs_uri_parse_path, shouldParsePath) {
    EXPECT_EQ(rcs::uri::parse(GetParam().in).path,
              GetParam().out);
}

INSTANTIATE_TEST_CASE_P(
    ,
    rcs_uri_parse_path,
    ::testing::Values(
        rcs_uri_parse_path_param("", "/"),
        rcs_uri_parse_path_param("://", "/"),
        rcs_uri_parse_path_param(":///", "/"),
        rcs_uri_parse_path_param("://?", "/"),
        rcs_uri_parse_path_param("://#", "/"),
        rcs_uri_parse_path_param(":///path", "/path"),
        rcs_uri_parse_path_param(":///nested/path", "/nested/path"),
        rcs_uri_parse_path_param(":///nested/path?", "/nested/path"),
        rcs_uri_parse_path_param(":///nested/path#", "/nested/path"),
        rcs_uri_parse_path_param("://host/nested/path", "/nested/path")));

/* * */

struct rcs_uri_parse_query_param {
    std::string                        in;
    std::map<std::string, std::string> out;

    friend auto operator<<(std::ostream &out, const struct rcs_uri_parse_query_param &param) -> std::ostream & {
        std::string query;
        for (auto it = param.out.begin(); it != param.out.end(); ++it) {
            if (it != param.out.begin())
                query += '&';
            query += (it->first + "=" + it->second);
        }
        return (out << std::format("{{in: '{}' | out: '{}'}}", param.in, query));
    }
};

class rcs_uri_parse_query : public ::testing::TestWithParam<struct rcs_uri_parse_query_param> {};

TEST_P(rcs_uri_parse_query, shouldParseQuery) {
    EXPECT_EQ(rcs::uri::parse(GetParam().in).query,
              GetParam().out);
}

// clang-format off
// clang-format is going crazy here for some reason
INSTANTIATE_TEST_CASE_P(
    ,
    rcs_uri_parse_query,
    ::testing::Values(
        rcs_uri_parse_query_param("", {}),
        rcs_uri_parse_query_param("", {}),
        rcs_uri_parse_query_param("://", {}),
        rcs_uri_parse_query_param("://?", {}),
        rcs_uri_parse_query_param("://?a=b", {{"a", "b"}}),
        rcs_uri_parse_query_param("://?a=b#", {{"a", "b"}}),
        rcs_uri_parse_query_param("://?a=b&b=c", {{"a", "b"}, {"b", "c"}}),
        rcs_uri_parse_query_param("://?a=b&b=c#", {{"a", "b"}, {"b", "c"}})));
// clang-format on

/* * */

struct rcs_uri_stringify_param {
    std::string uri;

    friend auto operator<<(std::ostream &out, const struct rcs_uri_stringify_param &param) -> std::ostream & {
        return (out << std::format("{{uri: '{}'}}", param.uri));
    }
};

class rcs_uri_stringify : public ::testing::TestWithParam<struct rcs_uri_stringify_param> {};

TEST_P(rcs_uri_stringify, shouldReturnExpectedURIString) {
    EXPECT_EQ(rcs::uri::parse(GetParam().uri).stringify(),
              GetParam().uri);
}

INSTANTIATE_TEST_CASE_P(
    ,
    rcs_uri_stringify,
    ::testing::Values(
        rcs_uri_stringify_param("scheme:///"),
        rcs_uri_stringify_param("scheme://host/"),
        rcs_uri_stringify_param("scheme://host/?a=b"),
        rcs_uri_stringify_param("scheme://host/?a=b&b=c"),
        rcs_uri_stringify_param("scheme://host/path"),
        rcs_uri_stringify_param("scheme://host/path?a=b"),
        rcs_uri_stringify_param("scheme://host/path?a=b&b=c")));
