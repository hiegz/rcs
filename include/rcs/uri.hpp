#ifndef RCS_URI_HPP
#define RCS_URI_HPP

#include <rcs/lc.hpp>

#include <map>
#include <string>

namespace rcs {

/// @brief ...
class uri final {
  public:
    /// @brief Construct from an existing URI.
    uri(const uri &other) = default;

    /// @brief Assign from an existing URI.
    auto operator=(const uri &other) -> uri & = default;

    /// @brief Construct from an existing URI.
    uri(uri &&other) = default;

    /// @brief Assign from an existing URI.
    auto operator=(uri &&other) -> uri & = default;

  public:
    /// @brief Default constructor.
    uri() = default;

    /// @brief Destroy resources allocated for a URI.
    ~uri() = default;

  public:
    /// @brief Parse a given URI string.
    static auto parse(const std::string &uri) -> rcs::uri;

  public:
    /// @brief Check if the URI is valid.
    auto is_valid() const -> bool;

  public:
    /// @brief Obtain the URI as a string.
    auto stringify() const -> std::string;

  public:
    /// @brief Percent-encode the given URI.
    template <rcs::lc TLetterCase = rcs::lc::lowercase>
    static auto encode(const std::string &in) -> std::string;

    /// @brief Percent-decode the given URI.
    static auto decode(const std::string &in) -> std::string;

  public:
    /// @brief ...
    std::string scheme;

    /// @brief ...
    std::string host;

    /// @brief ...
    std::string path = "/";

    /// @brief ...
    std::map<std::string, std::string> query;
};

} // namespace rcs

#endif
