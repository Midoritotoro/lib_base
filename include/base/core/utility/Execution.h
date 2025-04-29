#pragma once 

#include <base/core/arch/Platform.h>

__BASE_NAMESPACE_BEGIN

namespace execution {
    class SequencedPolicy {
        // indicates support for only sequential execution, and requests termination on exceptions
    public:
        using                   StandardExecutionPolicy = int;
        static constexpr bool   Parallelize             = false;
        static constexpr bool   Ivdep                   = false;
    };

    class ParallelPolicy {
        // indicates support by element access functions for parallel execution with parallel forward progress
        // guarantees, and requests termination on exceptions
    public:
        using                   StandardExecutionPolicy = int;
        static constexpr bool   Parallelize             = true;
        static constexpr bool   Ivdep                   = true;
    };

    class ParallelUnsequencedPolicy {
        // indicates support by element access functions for parallel execution with weakly parallel forward progress
        // guarantees, and requests termination on exceptions
        //
        // (at this time, equivalent to parallel_policy)
    public:
        using                   StandardExecutionPolicy = int;
        static constexpr bool   Parallelize             = true;
        static constexpr bool   Ivdep                   = true;
    };

    class UnsequencedPolicy {
        // indicates support by element access functions for weakly parallel forward progress guarantees, and for
        // executing interleaved on the same thread, and requests termination on exceptions
        //
        // (at this time, equivalent to sequenced_policy except for for_each(_n), destroy(_n),
        // uninitialized_default_construct(_n), and uninitialized_value_construct(_n))
    public:
        using                   StandardExecutionPolicy = int;
        static constexpr bool   Parallelize             = false;
        static constexpr bool   Ivdep                   = true;
    };

    inline constexpr SequencedPolicy            sequenced           {};
    inline constexpr UnsequencedPolicy          unsequenced         {};

    inline constexpr ParallelPolicy             parallel            {};
    inline constexpr ParallelUnsequencedPolicy  parallelUnsequenced {};
} // namespace execution


template <class _ExecutionPolicy_>
struct IsExecutionPolicy : 
    std::false_type
{};

template <>
struct IsExecutionPolicy<execution::SequencedPolicy> :
    std::true_type 
{};

template <>
struct IsExecutionPolicy<execution::ParallelPolicy> :
    std::true_type
{};

template <>
struct IsExecutionPolicy<execution::ParallelUnsequencedPolicy> :
    std::true_type
{};

template <>
struct IsExecutionPolicy<execution::UnsequencedPolicy> :
    std::true_type 
{};

__BASE_NAMESPACE_END