#ifndef RCS_IO_URING_SQ_OFF_HPP
#define RCS_IO_URING_SQ_OFF_HPP

#include <cstdint>

namespace rcs::io::uring {

/// @brief Submission queue offsets.
struct sq_off final {
    std::uint32_t head;
    std::uint32_t tail;
    std::uint32_t mask;
    std::uint32_t capacity;
    std::uint32_t flags;
    std::uint32_t dropped;
    std::uint32_t array;
    std::uint32_t resv1;
    std::uint64_t user_addr;
};

} // namespace rcs::io::uring

#endif
