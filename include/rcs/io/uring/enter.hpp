#ifndef RCS_IO_URING_ENTER_HPP
#define RCS_IO_URING_ENTER_HPP

#include <bits/types/sigset_t.h>
#include <cstdint>

namespace rcs::io::uring {

///
/// @brief   Initiate or wait for previously initiated I/O.
///
/// @details Submits a number of I/Os to the kernel or waits for completion of
///          previously initiated I/Os.
/// @details If `sig` is not `NULL`, `rcs::io::uring::enter` first replaces the
///          current signal mask by the one pointed to by `sig`, then waits
///          for events to become available in the completion queue, and then
///          restores the original signal mask.
///
/// @return  Returns the number of I/Os successfully consumed. This can be
///          zero if `submitnr` was zero or if the submission queue was empty.
///
/// @throws  rcs::system::exception
///
auto enter(std::int32_t  descriptor,
           std::uint32_t submitnr,
           std::uint32_t waitnr,
           std::uint32_t flags,
           ::sigset_t   *sig) -> std::uint32_t;

} // namespace rcs::io::uring

#endif
