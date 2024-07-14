/// @file

#ifndef RCS_IO_URING_OP_HPP
#define RCS_IO_URING_OP_HPP

#include <cstdint>

namespace rcs::io::uring {

/// @brief Specifies the operation to be performed.
enum class op : std::uint8_t {
    /// @details Do not perform any I/O.
    nop = 0,

    readv   = 1,
    writev  = 2,
    sendmsg = 9,
    recvmsg = 10,
    accept  = 13,
    connect = 16,
    read    = 22,
    write   = 23,
};

} // namespace rcs::io::uring

#endif
