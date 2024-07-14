/// @file

#ifndef RCS_IO_URING_FLAGS_HPP
#define RCS_IO_URING_FLAGS_HPP

#include <cstdint>

namespace rcs::io::uring {

/// @details Enables the application to specify the number of entries that
///          the completion queue can hold which must be greater than the
///          submission queue capacity. The specified value may be rounded
///          up to the next power-of-two.
static constexpr std::uint32_t SETUP_CQSIZE = 1U << 3;

/// @details Clamps specified queue capacities at the kernel-defined limits.
static constexpr std::uint32_t SETUP_CLAMP = 1U << 4;

/// @details Removes the indirection array. The submission queue will be
///          indexed directly by the submission queue tail.
static constexpr std::uint32_t SETUP_NO_SQARRAY = 1U << 16;

/// @details Waits for completion of the specified number of events.
static constexpr std::uint32_t ENTER_GETEVENTS = 1U << 0;

/// @details Prevents a submission queue entry from being processed before
///          previously submitted submission queue entries have completed.
///          New submission queue entries will not be started before the
///          submission queue entry with this flag completes.
static constexpr std::uint32_t SQE_IO_DRAIN = 1U << 1;

/// @details Prevents the next submission queue entry from being processed
///          before the preceding entry with this flag completes.
static constexpr std::uint32_t SQE_IO_LINK = 1U << 2;

/// @details Like SQE_IO_LINK, but it does not terminate the chain regardless
///          of the completion result.
static constexpr std::uint32_t SQE_IO_HARDLINK = 1U << 3;

} // namespace rcs::io::uring

#endif
