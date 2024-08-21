#ifndef RCS_URI_HPP
#define RCS_URI_HPP

#include <rcs/lc.hpp>

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

  private:
    /// @brief Default constructor.
    uri() = default;

    /// @brief Destroy resources allocated for a URI.
    ~uri() = default;

  public:
    /// @brief Percent-encode the given URI.
    template <rcs::lc TLetterCase = rcs::lc::lowercase>
    static auto encode(const std::string &in) -> std::string;

    /// @brief Percent-decode the given URI.
    static auto decode(const std::string &in) -> std::string;
};

} // namespace rcs

#endif
