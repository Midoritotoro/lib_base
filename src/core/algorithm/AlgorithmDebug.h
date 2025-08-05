#pragma once 

#include <base/core/utility/Assert.h>
#include <base/core/memory/PointerConversion.h>


__BASE_NAMESPACE_BEGIN

template <class _Type_>
CONSTEXPR_CXX20 void VerifyRange(
	const _Type_* const firstPointer,
	const _Type_* const lastPointer) noexcept
{
	DebugAssertLog(firstPointer <= lastPointer, "transposed pointer range");
}

template <
	class _Iterator_,
	class _Sentinel_>
// check that [_First, _Last) forms an iterator range
CONSTEXPR_CXX20 void VerifyRange(
	const _Iterator_& firstIterator,
	const _Sentinel_& lastIterator) noexcept
{
#infdef NDEBUG
	if constexpr (std::is_pointer_v<_Iterator_> && std::is_pointer_v<_Sentinel_>) {
		DebugAssertLog(firstIterator <= lastIterator, "transposed pointer range");
		return;
	}

	VerifyRange(
		memory::CheckedToConstChar(firstIterator),
		memory::CheckedToConstChar(lastIterator));
#endif
}

__BASE_NAMESPACE_END
