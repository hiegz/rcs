#include <rcs/atomic.hpp>

#include <rcs/io/uring/enter.hpp>
#include <rcs/io/uring/flags.hpp>
#include <rcs/io/uring/params.hpp>
#include <rcs/io/uring/sq_off.hpp>
#include <rcs/io/uring/sqe.hpp>
#include <rcs/io/uring/sqr.hpp>

#include <rcs/system/exception.hpp>

#include <sys/mman.h>

#include <cassert>
#include <cerrno>
#include <cstdint>

rcs::io::uring::sqr::sqr(
    rcs::io::uring::sqr &&other) noexcept
    : m_map(other.m_map),
      m_ring(other.m_ring),
      m_shared(other.m_shared),
      m_tail(other.m_tail),
      m_mask(other.m_mask),
      m_flags(other.m_flags),
      m_descriptor(other.m_descriptor) {
    other._release();
}

auto rcs::io::uring::sqr::operator=(
    rcs::io::uring::sqr &&other) noexcept
    -> rcs::io::uring::sqr & {
    _reset();

    m_map        = other.m_map;
    m_ring       = other.m_ring;
    m_shared     = other.m_shared;
    m_tail       = other.m_tail;
    m_mask       = other.m_mask;
    m_flags      = other.m_flags;
    m_descriptor = other.m_descriptor;

    other._release();

    return *this;
}

rcs::io::uring::sqr::sqr(
    std::int32_t                  descriptor,
    const rcs::io::uring::params &params)
    : rcs::io::uring::sqr() {
    m_descriptor = descriptor;

    m_map.size = params.sq_off.array + (params.sq_capacity * sizeof(std::uint32_t));
    m_map.base = reinterpret_cast<std::uint8_t *>(
        ::mmap(
            nullptr, m_map.size,
            PROT_READ | PROT_WRITE, MAP_SHARED | MAP_POPULATE,
            m_descriptor, 0ULL));
    if (m_map.base == MAP_FAILED) throw rcs::system::exception(errno);

    // m_ring.size  = params.sq_capacity;
    m_ring.size = params.sq_capacity * sizeof(rcs::io::uring::sqe);
    m_ring.base = reinterpret_cast<rcs::io::uring::sqe *>(
        ::mmap(
            nullptr, m_ring.size,
            PROT_READ | PROT_WRITE, MAP_SHARED | MAP_POPULATE,
            m_descriptor, 0x10000000ULL));
    if (m_ring.base == MAP_FAILED) throw rcs::system::exception(errno);

    m_shared.head = reinterpret_cast<std::uint32_t *>(m_map.base + params.sq_off.head);
    m_shared.tail = reinterpret_cast<std::uint32_t *>(m_map.base + params.sq_off.tail);
    m_tail        = *reinterpret_cast<std::uint32_t *>(m_map.base + params.sq_off.tail);
    m_mask        = *reinterpret_cast<std::uint32_t *>(m_map.base + params.sq_off.mask);
    m_flags       = *reinterpret_cast<std::uint32_t *>(m_map.base + params.sq_off.flags);

    if ((params.flags & rcs::io::uring::SETUP_NO_SQARRAY) == 0) {
        std::uint32_t *array =
            reinterpret_cast<std::uint32_t *>(
                m_map.base + params.sq_off.array);

        // Fill out the indirection array such that the queue can be indexed
        // directly by the submission queue tail.
        for (std::uint32_t index = 0; index < params.sq_capacity; ++index)
            array[index] = index;
    }
}

auto rcs::io::uring::sqr::empty() const
    -> bool { return pending() == 0; }

auto rcs::io::uring::sqr::full() const
    -> bool { return pending() == capacity(); }

auto rcs::io::uring::sqr::pending() const
    -> std::uint32_t { return m_tail - rcs::atomic::acquire(m_shared.head); }

auto rcs::io::uring::sqr::capacity() const
    -> std::uint32_t { return m_ring.size / sizeof(rcs::io::uring::sqe); }

auto rcs::io::uring::sqr::next()
    -> rcs::io::uring::sqe & {
    assert(not full());

    rcs::io::uring::sqe *entry =
        &m_ring.base[m_tail++ & m_mask];
    *entry = rcs::io::uring::sqe();

    return *entry;
}

auto rcs::io::uring::sqr::submit()
    const -> std::uint32_t {
    assert(not empty());

    rcs::atomic::release(m_shared.tail, m_tail);
    return rcs::io::uring::enter(
        m_descriptor,
        sqr::pending(),
        0, 0, nullptr);
}

void rcs::io::uring::sqr::_release() {
    m_map        = {};
    m_ring       = {};
    m_shared     = {};
    m_tail       = 0;
    m_mask       = 0;
    m_flags      = 0;
    m_descriptor = -1;
}

void rcs::io::uring::sqr::_reset() {
    if (m_ring.base != nullptr)
        ::munmap(m_ring.base, m_ring.size);
    if (m_map.base != nullptr)
        ::munmap(m_map.base, m_map.size);
    _release();
}

rcs::io::uring::sqr::~sqr() {
    _reset();
}
