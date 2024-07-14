#include <rcs/co/awaitable.hpp>
#include <rcs/log.hpp>

#include <exception>

auto co_success() -> rcs::co::awaitable<int> { co_return 0; } // NOLINT
auto co_failure() -> rcs::co::awaitable<int> { co_return 1; } // NOLINT

auto co_print() -> rcs::co::awaitable<void> {
    rcsinfo("Running {} v{} [{} {} ({})]",
            CMAKE_PROJECT_NAME,
            CMAKE_PROJECT_VERSION,
            CMAKE_BUILD_TIMESTAMP,
            CMAKE_BUILD_TYPE,
            RCS_LOG_SEVERITY);

    co_return;
}

auto co_main(int argc, char **argv) -> rcs::co::awaitable<int> {
    (void)argc, (void)argv;
    co_await co_print();
    co_return (co_await co_success());
}

auto main(int argc, char **argv) -> int {
    auto _m = co_main(argc, argv);

    try {
        return _m.result();
    } catch (const std::exception &e) {
        rcsfatal("Exception caught : {}", e.what());
        return 1;
    }
}
