#ifndef RCS_IO_URING_CQ_OFF_HPP
#define RCS_IO_URING_CQ_OFF_HPP

#include <cstdint>

namespace rcs::io::uring {

/// @brief Completion queue offsets.
class cq_off final {
  public:
    std::uint32_t head     = 0;
    std::uint32_t tail     = 0;
    std::uint32_t mask     = 0;
    std::uint32_t capacity = 0;
    std::uint32_t overflow = 0;
    std::uint32_t ring     = 0;
    std::uint32_t flags    = 0;

  private:
    [[maybe_unused]]
    std::uint8_t _m_pad1[4] = {0};

  public:
    std::uint64_t token = 0;
};

} // namespace rcs::io::uring

#endif
