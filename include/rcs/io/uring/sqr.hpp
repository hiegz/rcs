#ifndef RCS_IO_URING_SQR_HPP
#define RCS_IO_URING_SQR_HPP

#include <rcs/io/uring/params.hpp> // IWYU pragma: keep
#include <rcs/io/uring/sqe.hpp>    // IWYU pragma: keep

#include <cstdint>

namespace rcs::io::uring {

/// @brief Submission queue ring.
class sqr final {
  public:
    static constexpr std::uint32_t MAX_CAPACITY = 32768;

  public:
    sqr(const sqr &other)                     = delete;
    auto operator=(const sqr &other) -> sqr & = delete;

  public:
    /// @brief Construct from an existing submission queue ring.
    sqr(sqr &&other) noexcept;

    /// @brief Assign from an existing submission queue ring.
    auto operator=(sqr &&other) noexcept -> sqr &;

    /// @brief Construct a submission queue ring.
    sqr(std::int32_t descriptor, const rcs::io::uring::params &params);

    /// @brief Default constructor.
    sqr() = default;

    /// @brief Default destructor.
    ~sqr();

  public:
    /// @brief Check whether the queue is empty.
    [[nodiscard]] auto empty() const -> bool;

    /// @brief Check whether the queue is full.
    [[nodiscard]] auto full() const -> bool;

    /// @brief Get the number of pending entries.
    [[nodiscard]] auto pending() const -> std::uint32_t;

    /// @brief Get the submission queue capacity.
    [[nodiscard]] auto capacity() const -> std::uint32_t;

    /// @brief Get the next entry in the submission queue.
    [[nodiscard]] auto next() -> rcs::io::uring::sqe &;

  public:
    /// @brief   Submit the next submission entries to the kernel.
    auto submit() const -> std::uint32_t;

  private:
    /// @brief Release ownership over allocated resources, if any.
    void _release();

    /// @brief Free the allocated resources, if any.
    void _reset();

  private:
    struct map_t {
        std::uint8_t *base = nullptr;
        std::uint32_t size = 0;
    };

    /// @brief Info about the mapped memory region.
    struct sqr::map_t m_map = {};

  private:
    struct ring_t {
        rcs::io::uring::sqe *base = nullptr;
        std::uint32_t        size = 0;
    };

    /// @brief Info about the queue ring.
    struct sqr::ring_t m_ring = {};

  private:
    struct shared_t {
        std::uint32_t *head = nullptr;
        std::uint32_t *tail = nullptr;
    };

    /// @brief Shared state.
    struct sqr::shared_t m_shared = {};

    /// @brief Intermediate tail state.
    std::uint32_t m_tail = 0;

    /// @brief Ring mask.
    std::uint32_t m_mask = 0;

    /// @brief Ring flags.
    std::uint32_t m_flags = 0;

  private:
    /// @brief io_uring instance identifier.
    std::int32_t m_descriptor = -1;
};

} // namespace rcs::io::uring

#endif
