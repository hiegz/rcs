#include <rcs/hex.hpp>
#include <rcs/lc.hpp>
#include <rcs/uri.hpp>

#include <string>

#include <cstdint>

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

template auto rcs::uri::encode<rcs::lc::lowercase>(const std::string &in) -> std::string;
template auto rcs::uri::encode<rcs::lc::uppercase>(const std::string &in) -> std::string;
