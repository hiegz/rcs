#ifndef RCS_IO_URING_CQR_HPP
#define RCS_IO_URING_CQR_HPP

#include <rcs/io/uring/cqe.hpp>    // IWYU pragma: keep
#include <rcs/io/uring/params.hpp> // IWYU pragma: keep

#include <cstdint>

namespace rcs::io::uring {

/// @brief Completion queue ring.
class cqr final {
  public:
    static constexpr std::uint32_t MAX_CAPACITY = 65536;

  public:
    cqr(const cqr &other)                     = delete;
    auto operator=(const cqr &other) -> cqr & = delete;

  public:
    /// @brief Construct from an existing completion queue ring.
    cqr(cqr &&other) noexcept;

    /// @brief Assign from an existing completion queue ring.
    auto operator=(cqr &&other) noexcept -> cqr &;

    /// @brief Construct a completion queue ring.
    cqr(std::int32_t descriptor, const rcs::io::uring::params &params);

    /// @brief Default constructor.
    cqr() = default;

    /// @brief Default destructor.
    ~cqr();

  public:
    /// @brief Check whether the queue is empty.
    [[nodiscard]] auto empty() const -> bool;

    /// @brief Check whether the queue is full.
    [[nodiscard]] auto full() const -> bool;

    /// @brief Get the number of pending entries.
    [[nodiscard]] auto pending() const -> std::uint32_t;

    /// @brief Get the completion queue capacity.
    [[nodiscard]] auto capacity() const -> std::uint32_t;

  public:
    /// @brief Wait for completion of a specified number of events.
    void wait(std::uint32_t waitnr) const;

    /// @brief Retrieve the next entry in the queue.
    [[nodiscard]] auto next()
        -> const rcs::io::uring::cqe &;

    /// @brief Mark all retrieved event as consumed.
    void seen() const;

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
    struct cqr::map_t m_map = {};

  private:
    struct ring_t {
        rcs::io::uring::cqe *base = nullptr;
        std::uint32_t        size = 0;
    };

    /// @brief Info about the queue ring.
    struct cqr::ring_t m_ring = {};

  private:
    struct shared_t {
        std::uint32_t *head = nullptr;
        std::uint32_t *tail = nullptr;
    };

    /// @brief Shared state.
    struct cqr::shared_t m_shared = {};

    /// @brief Intermediate head state.
    std::uint32_t m_head = 0;

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
