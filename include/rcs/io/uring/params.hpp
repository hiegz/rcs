#ifndef RCS_IO_URING_PARAMS_HPP
#define RCS_IO_URING_PARAMS_HPP

#include <rcs/io/uring/cq_off.hpp>
#include <rcs/io/uring/sq_off.hpp>

#include <cstdint>

namespace rcs::io::uring {

/// @brief Information about an io_uring instance.
class params {
  public:
    /// @brief Assign from existing parameters.
    auto operator=(const params &other) -> params & = default;

    /// @brief Construct from existing parameters.
    params(const params &other) = default;

    /// @brief Construct parameters.
    params() = default;

    /// @brief Default destructor.
    ~params() = default;

  public:
    /// @brief Submission queue capacity.
    std::uint32_t sq_capacity = 0;

    /// @brief Completion queue capacity.
    std::uint32_t cq_capacity = 0;

    std::uint32_t flags          = 0;
    std::uint32_t sq_thread_cpu  = 0;
    std::uint32_t sq_thread_idle = 0;

    /// @brief A set of features supported by the current kernel version.
    std::uint32_t features = 0;

  private:
    [[maybe_unused]] std::uint32_t m_wq_fd   = 0;
    [[maybe_unused]] std::uint32_t m_resv[3] = {0};

  public:
    /// @brief Submission queue offsets.
    rcs::io::uring::sq_off sq_off = {};

    /// @brief Completion queue offsets.
    rcs::io::uring::cq_off cq_off = {};
};

} // namespace rcs::io::uring

#endif
