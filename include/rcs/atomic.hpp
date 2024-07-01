#ifndef RCS_ATOMIC_HPP
#define RCS_ATOMIC_HPP

#include <atomic>

namespace rcs::atomic {

template <std::memory_order M = std::memory_order::relaxed, typename T>
inline auto load(const T *ptr) -> T {
    return std::atomic_load_explicit(
        reinterpret_cast<const std::atomic<T> *>(ptr), M);
}

template <typename T>
inline auto acquire(const T *ptr) -> T {
    return rcs::atomic::load<
        std::memory_order::acquire>(ptr);
}

template <std::memory_order M = std::memory_order::relaxed, typename T>
inline void store(T *ptr, T value) {
    std::atomic_store_explicit(
        reinterpret_cast<std::atomic<T> *>(ptr), value, M);
}

template <typename T>
inline void release(T *ptr, T value) {
    return rcs::atomic::store<
        std::memory_order::release>(ptr, value);
}

} // namespace rcs::atomic

#endif
