#include <rcs/system/handle.hpp>

#include <unistd.h>

#include <cassert>

rcs::system::handle::handle(handle &&other) noexcept
    : m_descriptor(other.m_descriptor) {
    other.m_descriptor = -1;
}

auto rcs::system::handle::operator=(handle &&other) noexcept
    -> handle & {
    handle::reset(other.m_descriptor);
    other.m_descriptor = -1;
    return *this;
}

rcs::system::handle::handle(handle::type descriptor)
    : m_descriptor(descriptor) {}

auto rcs::system::handle::descriptor() const
    -> const handle::type & { return m_descriptor; }

auto rcs::system::handle::release()
    -> handle::type {
    const int descriptor = m_descriptor;
    m_descriptor         = -1;
    return descriptor;
}

void rcs::system::handle::reset(handle::type descriptor) {
    if (-1 != m_descriptor) {
        [[maybe_unused]] const int ret =
            ::close(m_descriptor);
        assert(-1 != ret);
    }

    m_descriptor = descriptor;
}

rcs::system::handle::~handle() {
    handle::reset(-1);
}
