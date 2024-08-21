#ifndef RCS_LC_HPP
#define RCS_LC_HPP

#include <cstdint>

namespace rcs {

/// @brief Letter case.
enum class lc : std::uint8_t {
    lowercase = 0,
    uppercase = 1,
};

} // namespace rcs

#endif
