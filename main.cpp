#include <iostream>

auto main(int /*argc*/, char ** /*argv*/) -> int {
    std::cout << CMAKE_PROJECT_NAME << " v"
              << CMAKE_PROJECT_VERSION << " ["
              << CMAKE_BUILD_TIMESTAMP << " "
              << CMAKE_BUILD_TYPE << "]\n";
}
