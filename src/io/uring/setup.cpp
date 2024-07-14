#include <rcs/io/uring/setup.hpp>
#include <rcs/system/exception.hpp>

#include <asm/unistd_64.h>
#include <unistd.h>

#include <cerrno>
#include <cstdint>

namespace rcs::io::uring {
class params;
}

auto rcs::io::uring::setup(std::uint32_t capacity, rcs::io::uring::params &params)
    -> std::int32_t {
    const std::int32_t descriptor = static_cast<std::int32_t>(syscall(__NR_io_uring_setup, capacity, &params));
    if (descriptor == -1) throw rcs::system::exception(errno);
    return descriptor;
}
