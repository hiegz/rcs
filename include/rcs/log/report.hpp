#ifndef RCS_LOG_REPORT_HPP
#define RCS_LOG_REPORT_HPP

#include <rcs/log/backtrace.hpp> // IWYU pragma: keep
#include <rcs/log/severity.hpp>  // IWYU pragma: keep

#include <string>

namespace rcs::log {

/// @brief Log the given message.
template <enum rcs::log::severity>
void report(const std::string &what);

/// @brief Log the given message with the backtrace information.
template <enum rcs::log::severity>
void report(const std::string         &what,
            struct rcs::log::backtrace backtrace);

} // namespace rcs::log

#endif
