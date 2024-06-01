#ifndef RCS_LOG_HPP
#define RCS_LOG_HPP

#include <rcs/log/backtrace.hpp> // IWYU pragma: keep
#include <rcs/log/report.hpp>    // IWYU pragma: keep
#include <rcs/log/severity.hpp>  // IWYU pragma: keep

#include <format>                // IWYU pragma: keep

#ifndef RCS_LOG_SEVERITY
#define RCS_LOG_SEVERITY (RCS_LOG_SEVERITY_TRACE)
#endif

#if RCS_LOG_SEVERITY_FATAL <= RCS_LOG_SEVERITY
#define rcsfatal(...)                            \
    rcs::log::report<rcs::log::severity::fatal>( \
        std::format(__VA_ARGS__))
#else
#define rcsfatal(...)
#endif

#if RCS_LOG_SEVERITY_ERROR <= RCS_LOG_SEVERITY
#define rcserror(...)                            \
    rcs::log::report<rcs::log::severity::error>( \
        std::format(__VA_ARGS__))
#else
#define rcserror(...)
#endif

#if RCS_LOG_SEVERITY_WARN <= RCS_LOG_SEVERITY
#define rcswarn(...)                            \
    rcs::log::report<rcs::log::severity::warn>( \
        std::format(__VA_ARGS__))
#else
#define rcswarn(...)
#endif

#if RCS_LOG_SEVERITY_INFO <= RCS_LOG_SEVERITY
#define rcsinfo(...)                            \
    rcs::log::report<rcs::log::severity::info>( \
        std::format(__VA_ARGS__))
#else
#define rcsinfo(...)
#endif

#if RCS_LOG_SEVERITY_DEBUG <= RCS_LOG_SEVERITY
#define rcsdebug(...)                            \
    rcs::log::report<rcs::log::severity::debug>( \
        std::format(__VA_ARGS__),                \
        rcs_log_backtrace_here)
#else
#define rcsdebug(...)
#endif

#if RCS_LOG_SEVERITY_TRACE <= RCS_LOG_SEVERITY
#define rcstrace(...)                            \
    rcs::log::report<rcs::log::severity::trace>( \
        std::format(__VA_ARGS__),                \
        rcs_log_backtrace_here)
#else
#define rcstrace(...)
#endif

#endif
