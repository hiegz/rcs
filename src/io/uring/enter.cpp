#include <rcs/io/uring/enter.hpp>
#include <rcs/system/exception.hpp>

#include <asm/unistd_64.h>
#include <bits/types/sigset_t.h>
#include <unistd.h>

#include <cerrno>
#include <cstdint>

auto rcs::io::uring::enter(
    std::int32_t  descriptor,
    std::uint32_t submitnr,
    std::uint32_t waitnr,
    std::uint32_t flags,
    ::sigset_t   *sig) -> std::uint32_t {
    const int consumed = (int)syscall(
        __NR_io_uring_enter,
        descriptor,
        submitnr,
        waitnr,
        flags, sig, 0);
    if (consumed == -1) throw rcs::system::exception(errno);
    return static_cast<std::uint32_t>(consumed);
}
