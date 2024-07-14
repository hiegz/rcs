#ifndef RCS_IO_URING_SQE_HPP
#define RCS_IO_URING_SQE_HPP

#include <rcs/io/uring/op.hpp>

#include <cstdint>

struct iovec;
struct msghdr;
struct sockaddr;

namespace rcs::io::uring {

/// @brief Submission queue entry.
class sqe final {
  public:
    /// @brief Construct from another entry.
    sqe(const sqe &other) = default;

    /// @brief Assign from another entry.
    auto operator=(const sqe &other) -> sqe & = default;

    /// @brief Construct an empty submission queue entry.
    sqe() = default;

    /// @brief Default destructor.
    ~sqe() = default;

  public:
    /// @brief Operation type.
    rcs::io::uring::op opcode = rcs::io::uring::op::nop;

    /// @brief Entry-specific flags.
    std::uint8_t flags = 0;

    /// @brief Operation priority.
    std::uint16_t priority = 0;

    /// @brief Operation resource identifier.
    std::int32_t descriptor = -1;

    union {
        std::uint8_t _m_def1[8] = {0};

        /// @brief Offset into a file.
        std::uint64_t offset;

        /// @brief Address length.
        std::uint32_t addrlen;

        /// @brief Address length pointer.
        std::uint32_t *addrlen2;
    };

    union {
        std::uint8_t _m_def2[8] = {0};

        /// @brief Memory buffer.
        void *buffer;

        /// @brief Scatter/gather buffers.
        struct ::iovec *iov;

        /// @brief Pointer to a socket address.
        struct ::sockaddr *address;

        /// @brief Pointer to a message structure.
        struct ::msghdr *msg;
    };

    union {
        std::uint8_t _m_def3[4] = {0};

        /// @brief Memory buffer size.
        std::uint32_t bufsize;

        /// @brief Number of scatter/gather buffers.
        std::uint32_t iovnr;
    };

  private:
    [[maybe_unused]] std::uint8_t _m_pad1[4] = {0};

  public:
    /// @brief Asynchronous completion token.
    std::uint64_t token = 0;

  private:
    [[maybe_unused]] std::uint8_t _m_pad2[2]  = {0};
    [[maybe_unused]] std::uint8_t _m_pad3[22] = {0};
};

} // namespace rcs::io::uring

#endif
