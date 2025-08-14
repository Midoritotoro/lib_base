#pragma once

#include <base/core/compatibility/CompilerDetection.h>
#include <base/core/arch/SystemDetection.h>

#if defined(base_os_windows) && defined(base_cpp_msvc) && !defined(base_cpp_gnu) && !defined(base_cpp_clang)
#include <__msvc_iter_core.hpp>
#endif

#if base_has_cxx20
	#include <ranges>
#endif

#include <base/core/utility/Execution.h>
#include <base/core/type_traits/TypeTraits.h>

//#include <src/core/utility/algorithm/AllOf.h>
//#include <src/core/utility/algorithm/NoneOf.h>

//#include <src/core/utility/algorithm/Copy.h>
//#include <src/core/utility/algorithm/Count.h>
//
//#include <src/core/utility/algorithm/Fill.h>
//#include <src/core/utility/algorithm/Find.h>
//
//#include <src/core/utility/algorithm/ForEach.h>
//#include <src/core/utility/algorithm/Mismatch.h>
//
//#include <src/core/utility/algorithm/Move.h>
//#include <src/core/utility/algorithm/PartialSort.h>

//#include <src/core/utility/algorithm/AnyOf.h>
//#include <src/core/utility/algorithm/Remove.h>
//
//#include <src/core/utility/algorithm/Replace.h>
//#include <src/core/utility/algorithm/Reverse.h>
//
//#include <src/core/utility/algorithm/ReverseCopy.h>
//#include <src/core/utility/algorithm/Rotate.h>
//
//#include <src/core/utility/algorithm/Search.h>
//#include <src/core/utility/algorithm/Sort.h>
//
//#include <src/core/utility/algorithm/SwapRanges.h>
//#include <src/core/utility/algorithm/Transform.h>

__BASE_NAMESPACE_BEGIN

using namespace ::std::ranges;

template <typename T>
void accumulateMax(T& a, const T& b) {
	if (a < b)
		a = b;
}

template <typename T>
void accumulateMin(T& a, const T& b) {
	if (a > b)
		a = b;
}

template <typename T>
T&& take(T& value) {
	return ::std::exchange(value, T{});
}

//#if defined(base_os_windows)
//	#include <string>
//	  bool IsWindowsGreaterThen(int version);
//	  bool SetAutoRunKey(
//		const ::std::wstring& path,
//		const ::std::wstring& key);
//
//	#define MINIMUM_WINDOWS_VERSION		NTDDI_WIN10
//	#define IS_MINIMUM_WINDOWS_VERSION	IsWindowsGreaterThen(MINIMUM_WINDOWS_VERSION)
//#endif // base_os_windows

__BASE_NAMESPACE_END
