#ifndef RCS_LOG_SEVERITY_HPP
#define RCS_LOG_SEVERITY_HPP

#include <cstdint>

#define RCS_LOG_SEVERITY_OFF   (0)
#define RCS_LOG_SEVERITY_FATAL (1)
#define RCS_LOG_SEVERITY_ERROR (2)
#define RCS_LOG_SEVERITY_WARN  (3)
#define RCS_LOG_SEVERITY_INFO  (4)
#define RCS_LOG_SEVERITY_DEBUG (5)
#define RCS_LOG_SEVERITY_TRACE (6)

namespace rcs::log {

/// @brief Log severity level.
enum class severity : std::uint8_t {
    /// @brief Disables log messages.
    off = RCS_LOG_SEVERITY_OFF,

    /// @brief Very severe error event that will presumably lead to a
    ///        service shutdown
    fatal = RCS_LOG_SEVERITY_FATAL,

    /// @brief Error event that might still allow the service to continue
    ///        running.
    error = RCS_LOG_SEVERITY_ERROR,

    /// @brief Event that might possibly lead to an error.
    warn = RCS_LOG_SEVERITY_WARN,

    /// @brief Informational messages that highlight the progress of the
    ///        service at coarse-grained level.
    info = RCS_LOG_SEVERITY_INFO,

    /// @brief Fine-grained informational events that are most useful to
    ///        debug the service.
    debug = RCS_LOG_SEVERITY_DEBUG,

    /// @brief Fine-grained informational events, typically capturing the flow
    ///        through the service.
    trace = RCS_LOG_SEVERITY_TRACE,
};

} // namespace rcs::log

#endif
