#include <rcs/log.hpp>

auto main(int /*argc*/, char ** /*argv*/) -> int {
    rcsinfo("Running {} v{} [{} {} ({})]",
            CMAKE_PROJECT_NAME,
            CMAKE_PROJECT_VERSION,
            CMAKE_BUILD_TIMESTAMP,
            CMAKE_BUILD_TYPE,
            RCS_LOG_SEVERITY);
}
