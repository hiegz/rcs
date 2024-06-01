#include <rcs/log/backtrace.hpp>
#include <rcs/log/report.hpp>
#include <rcs/log/severity.hpp>

#include <format>
#include <sstream>
#include <string>

#include <ctime>

#include <iomanip>
#include <iostream>

struct tm;

template <enum rcs::log::severity S>
void rcs::log::report(const std::string &what) {
    std::ostream &ostream = []() -> std::ostream & {
        switch (S) {
            case rcs::log::severity::off:
            case rcs::log::severity::fatal:
            case rcs::log::severity::error:
            case rcs::log::severity::warn:
                return std::cerr;
            case rcs::log::severity::info:
            case rcs::log::severity::debug:
            case rcs::log::severity::trace:
            default:
                return std::cout;
        }
    }();

    const std::string timestamp = []() -> std::string {
        std::stringstream sstream;
        const std::tm    *time_ptr = nullptr;
        std::time_t       time     = 0;

        (void)std::time(&time);
        time_ptr = std::localtime(&time);

        sstream << std::put_time(time_ptr, "%a %F %H:%M:%S");
        return std::format("[{}]", sstream.str());
    }();

    const std::string tag = []() -> std::string {
        switch (S) {
            case rcs::log::severity::off:
                return "[]";
            case rcs::log::severity::fatal:
                return "[FATAL]";
            case rcs::log::severity::error:
                return "[ERROR]";
            case rcs::log::severity::warn:
                return "[WARN]";
            case rcs::log::severity::info:
                return "[INFO]";
            case rcs::log::severity::debug:
                return "[DEBUG]";
            case rcs::log::severity::trace:
                return "[TRACE]";
            default:
                return "[]";
        }
    }();

    ostream << std::format("{} {} {}\n", timestamp, tag, what);
}

template <enum rcs::log::severity S>
void rcs::log::report(const std::string         &what,
                      struct rcs::log::backtrace backtrace) {
    const std::string backtrace_str =
        std::format(
            "[{}:{}():{}]",
            backtrace.file,
            backtrace.func,
            backtrace.line);

    rcs::log::report<S>(std::format("{} {}", backtrace_str, what));
}

template void rcs::log::report<rcs::log::severity::fatal>(const std::string &);
template void rcs::log::report<rcs::log::severity::error>(const std::string &);
template void rcs::log::report<rcs::log::severity::warn>(const std::string &);
template void rcs::log::report<rcs::log::severity::info>(const std::string &);
template void rcs::log::report<rcs::log::severity::debug>(const std::string &);
template void rcs::log::report<rcs::log::severity::trace>(const std::string &);

template void rcs::log::report<rcs::log::severity::fatal>(const std::string &, struct rcs::log::backtrace);
template void rcs::log::report<rcs::log::severity::error>(const std::string &, struct rcs::log::backtrace);
template void rcs::log::report<rcs::log::severity::warn>(const std::string &, struct rcs::log::backtrace);
template void rcs::log::report<rcs::log::severity::info>(const std::string &, struct rcs::log::backtrace);
template void rcs::log::report<rcs::log::severity::debug>(const std::string &, struct rcs::log::backtrace);
template void rcs::log::report<rcs::log::severity::trace>(const std::string &, struct rcs::log::backtrace);
