#ifndef RCS_IO_URING_SETUP_HPP
#define RCS_IO_URING_SETUP_HPP

#include <cstdint>

namespace rcs::io::uring {

class params;

///
/// @brief   Setup a context for performing asynchronous I/O.
///
/// @returns On success, returns a new file descriptor which the application
///          may use to map submission and completion queues.
///
/// @throws  rcs::system::exception
///
auto setup(std::uint32_t capacity, rcs::io::uring::params &params)
    -> std::int32_t;

} // namespace rcs::io::uring

#endif
