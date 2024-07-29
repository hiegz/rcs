#ifndef RCS_EXECUTION_EXECUTOR_HPP
#define RCS_EXECUTION_EXECUTOR_HPP

#include <concepts>
#include <type_traits>

#include <rcs/execution/invocable_archetype.hpp> // IWYU pragma: keep

namespace rcs::execution {

template <class TExecutorType, class TWork>
concept executor_of_work =
    std::invocable<std::decay_t<TWork> &> &&
    std::constructible_from<std::decay_t<TWork>, TWork> &&
    std::move_constructible<std::decay_t<TWork>> &&
    std::copy_constructible<TExecutorType> &&
    std::is_default_constructible_v<TExecutorType> &&
    std::is_nothrow_copy_constructible_v<TExecutorType> &&
    std::equality_comparable<TExecutorType> /* nothrow */ &&
    requires(TExecutorType &executor, TWork &&work) {
        executor.execute((TWork &&)work);
    };

template <typename TExecutorType>
concept executor =
    rcs::execution::executor_of_work<
        TExecutorType, rcs::execution::invokable_archetype>;

template <typename TExecutorType, typename TWork>
concept executor_of =
    rcs::execution::executor<TExecutorType> &&
    rcs::execution::executor_of_work<TExecutorType, TWork>;

} // namespace rcs::execution

#endif
