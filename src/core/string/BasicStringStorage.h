#pragma once 

#include <base/core/arch/Platform.h>

__BASE_STRING_NAMESPACE_BEGIN

struct StringSimd {
	struct _Optimization_ 
	{};
	// If SSE4.2 is supported on the device being launched,
	// then these instructions are used.
	// Otherwise, it is similar to _Optimization_Disable_
	struct _Optimization_Enable_Only_If_SSE42_ : _Optimization_
	{};

	// If AVX is supported on the device being launched,
	// then these instructions are used.
	// Otherwise, it is similar to _Optimization_Disable_
	struct _Optimization_Enable_Only_If_AVX_ : _Optimization_
	{};

	// If AVX512 is supported on the device being launched,
	// then these instructions are used.
	// Otherwise, it is similar to _Optimization_Disable_
	struct _Optimization_Enable_Only_If_AVX512_ : _Optimization_
	{};

	// Auto-selecting the most efficient option
	struct _Optimization_Auto_ : _Optimization_
	{};

	// A fully scalar string
	struct _Optimization_Disable_ : _Optimization_
	{};
};

enum class _String_Simd_Algorithm_Alignment
	: sizetype
{};

template <
	class _Char_,
	class _SimdOptimization_ = StringSimd::_Optimization_Disable_>
class BasicStringStorage {
public:
};

__BASE_STRING_NAMESPACE_END
