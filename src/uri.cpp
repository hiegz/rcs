#include <rcs/hex.hpp>
#include <rcs/lc.hpp>
#include <rcs/uri.hpp>

#include <algorithm>
#include <cctype>
#include <format>
#include <iterator>
#include <string>

#include <cassert>
#include <cstdint>

/* * */

template <rcs::lc TLetterCase>
auto rcs::uri::encode(const std::string &in) -> std::string {
    std::string out;
    for (const char c : in) {
        if (('a' <= c && c <= 'z') ||
            ('A' <= c && c <= 'Z') ||
            ('0' <= c && c <= '9') ||
            ('-' == c || c == '.') ||
            ('_' == c || c == '~')) {
            out += c;
        } else if (c == ' ') {
            out += '+';
        } else {
            out += '%';
            out += rcs::hex::encode<TLetterCase>(c >> 4);
            out += rcs::hex::encode<TLetterCase>(c & 0xf);
        }
    }
    return out;
}

auto rcs::uri::decode(const std::string &in) -> std::string {
    std::string out;
    for (std::uint32_t i = 0; i < in.size(); ++i) {
        if (in[i] == '+') {
            out += ' ';
        } else if (in[i] == '%') {
            char c;
            rcs::hex::decode(&in[i + 1], 2, &c);
            out += c;

            // Jump over percent-encoded characters
            i += 2;
        } else {
            out += in[i];
        }
    }
    return out;
}

/* * */

template auto rcs::uri::encode<rcs::lc::lowercase>(const std::string &in) -> std::string;
template auto rcs::uri::encode<rcs::lc::uppercase>(const std::string &in) -> std::string;

/* * */

auto rcs::uri::parse(const std::string &uri) -> rcs::uri {
    rcs::uri                    out;
    std::string::const_iterator it[2] =
        {{}, {}};

    /* * */

    const std::string scheme_delimiter("://");
    it[0] = std::search(uri.begin(), uri.end(), scheme_delimiter.begin(), scheme_delimiter.end());
    if (it[0] != uri.end()) {
        out.scheme.reserve(std::distance(uri.begin(), it[0]));
        std::transform(uri.begin(), it[0], std::back_inserter(out.scheme), [](char c) { return std::tolower(c); });
        std::advance(it[0], scheme_delimiter.length());
    } else {
        it[0] = uri.begin();
    }

    //
    // [scheme://][host][/path][?query=something][#fragment]
    //             ^
    //            it[0]
    //

    it[1] = std::find_if(it[0], uri.end(), [](char c) { return c == '/' || c == '?' || c == '#'; });
    out.host.reserve(std::distance(it[0], it[1]));
    std::transform(it[0], it[1], std::back_inserter(out.host), [](char c) { return std::tolower(c); });

    //
    // [scheme://][host][/path][?query=something][#fragment]
    //             ^     ^
    //           it[0] it[1]
    //

    it[0] = std::find_if(it[1], uri.end(), [](char c) { return c == '?' || c == '#'; });
    if (it[1] != uri.end() && *it[1] == '/')
        out.path = {it[1], it[0]};

    if (it[0] == uri.end() || *it[0] == '#')
        return out;

    //
    // [scheme://][host][/path][?query=something][#fragment]
    //                          ^
    //                        it[0]
    //

    std::string  qstr = {it[0] + 1, std::find(it[0], uri.end(), '#')};
    std::string  key;
    std::string  value;
    std::string *kv = &key;

    qstr += '&';

    for (const char c : qstr) {
        if (c == '=') kv = &value;
        else if (c == '&') {
            if (not key.empty() && not value.empty())
                out.query.insert({key, value});
            key   = "";
            value = "";
            kv    = &key;
        } else *kv += c;
    }

    /* * */

    return out;
}

auto rcs::uri::is_valid() const -> bool {
    return !scheme.empty() && !path.empty() && path[0] == '/';
}

auto rcs::uri::stringify() const -> std::string {
    std::string out;

    out += std::format("{}://{}{}", scheme, host, path);
    if (not query.empty())
        out += "?";
    for (auto it = query.begin(); it != query.end(); ++it) {
        if (it != query.begin())
            out += '&';
        out += (it->first + "=" + it->second);
    }

    return out;
}
