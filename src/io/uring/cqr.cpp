#include <rcs/atomic.hpp>

#include <rcs/io/uring/cq_off.hpp>
#include <rcs/io/uring/cqe.hpp>
#include <rcs/io/uring/cqr.hpp>
#include <rcs/io/uring/enter.hpp>
#include <rcs/io/uring/flags.hpp>
#include <rcs/io/uring/params.hpp>

#include <rcs/system/exception.hpp>

#include <sys/mman.h>

#include <cassert>
#include <cerrno>
#include <cstdint>

rcs::io::uring::cqr::cqr(
    rcs::io::uring::cqr &&other) noexcept
    : m_map(other.m_map),
      m_ring(other.m_ring),
      m_shared(other.m_shared),
      m_head(other.m_head),
      m_mask(other.m_mask),
      m_flags(other.m_flags),
      m_descriptor(other.m_descriptor) {
    other._release();
}

auto rcs::io::uring::cqr::operator=(
    rcs::io::uring::cqr &&other) noexcept
    -> rcs::io::uring::cqr & {
    _reset();

    m_map        = other.m_map;
    m_ring       = other.m_ring;
    m_shared     = other.m_shared;
    m_head       = other.m_head;
    m_mask       = other.m_mask;
    m_flags      = other.m_flags;
    m_descriptor = other.m_descriptor;

    other._release();

    return *this;
}

rcs::io::uring::cqr::cqr(
    int                           descriptor,
    const rcs::io::uring::params &params)
    : rcs::io::uring::cqr() {
    m_descriptor = descriptor;

    m_map.size = params.cq_off.ring + params.cq_capacity * sizeof(rcs::io::uring::cqe);
    m_map.base = reinterpret_cast<std::uint8_t *>(
        ::mmap(nullptr, m_map.size,
               PROT_READ | PROT_WRITE, MAP_SHARED | MAP_POPULATE,
               m_descriptor, 0x8000000ULL));
    if (m_map.base == MAP_FAILED) throw rcs::system::exception(errno);

    m_ring.base = reinterpret_cast<rcs::io::uring::cqe *>(m_map.base + params.cq_off.ring);
    m_ring.size = params.cq_capacity;

    m_shared.head = reinterpret_cast<std::uint32_t *>(m_map.base + params.cq_off.head);
    m_shared.tail = reinterpret_cast<std::uint32_t *>(m_map.base + params.cq_off.tail);
    m_head        = *reinterpret_cast<std::uint32_t *>(m_map.base + params.cq_off.head);
    m_mask        = *reinterpret_cast<std::uint32_t *>(m_map.base + params.cq_off.mask);
    m_flags       = *reinterpret_cast<std::uint32_t *>(m_map.base + params.cq_off.flags);
}

auto rcs::io::uring::cqr::empty() const
    -> bool { return cqr::pending() == 0; }

auto rcs::io::uring::cqr::full() const
    -> bool { return cqr::pending() == cqr::capacity(); }

auto rcs::io::uring::cqr::pending() const
    -> std::uint32_t {
    return rcs::atomic::acquire(m_shared.tail) -
           rcs::atomic::acquire(m_shared.head);
}

auto rcs::io::uring::cqr::capacity() const
    -> std::uint32_t { return m_ring.size / sizeof(rcs::io::uring::cqe); }

void rcs::io::uring::cqr::wait(std::uint32_t waitnr) const {
    (void)rcs::io::uring::enter(
        m_descriptor, 0, waitnr,
        rcs::io::uring::ENTER_GETEVENTS,
        nullptr);
}

auto rcs::io::uring::cqr::next()
    -> const rcs::io::uring::cqe & {
    assert(not empty());

    return m_ring.base[m_head++ & m_mask];
}

void rcs::io::uring::cqr::seen() const {
    rcs::atomic::release(m_shared.head, m_head);
}

void rcs::io::uring::cqr::_release() {
    m_map        = {};
    m_ring       = {};
    m_shared     = {};
    m_head       = 0;
    m_mask       = 0;
    m_flags      = 0;
    m_descriptor = -1;
}

void rcs::io::uring::cqr::_reset() {
    if (m_map.base != nullptr)
        ::munmap(m_map.base, m_map.size);
    _release();
}

rcs::io::uring::cqr::~cqr() {
    _reset();
}
