#ifndef RCS_EXECUTION_INVOCABLE_ARCHETYPE_HPP
#define RCS_EXECUTION_INVOCABLE_ARCHETYPE_HPP

namespace rcs::execution {

/// @brief An archetypal function object used for determining adherence to the
///        rcs::execution::executor concept.
struct invokable_archetype {
    template <typename... Args>
    void operator()(Args &&...);
};

} // namespace rcs::execution

#endif
