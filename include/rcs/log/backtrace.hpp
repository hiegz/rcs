#ifndef RCS_LOG_BACKTRACE_HPP
#define RCS_LOG_BACKTRACE_HPP

#include <cstring> // IWYU pragma: export

namespace rcs::log {

/// @brief Backtrace info about a log message.
struct backtrace {
    const char *file;
    const char *func;
    int         line;
};

#define rcs_log_backtrace_here                     \
    (rcs::log::backtrace{                          \
        .file = ::strrchr(__FILE__, '/')           \
                    ? ::strrchr(__FILE__, '/') + 1 \
                    : __FILE__,                    \
        .func = __FUNCTION__,                      \
        .line = __LINE__,                          \
    })

} // namespace rcs::log

#endif
