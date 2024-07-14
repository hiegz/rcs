#ifndef RCS_IO_URING_CQE_HPP
#define RCS_IO_URING_CQE_HPP

#include <cstdint>

namespace rcs::io::uring {

/// @brief Completion queue event.
class cqe final {
  public:
    /// @brief Construct from another completion queue event.
    cqe(const cqe &other) = default;

    /// @brief Assign from another completion queue event.
    auto operator=(const cqe &other) -> cqe & = default;

    /// @brief Construct an empty completion queue event.
    cqe() = default;

    /// @brief Default destructor.
    ~cqe() = default;

  public:
    std::uint64_t token  = 0;
    std::int32_t  result = 0;
    std::uint32_t flags  = 0;
};

} // namespace rcs::io::uring

#endif
