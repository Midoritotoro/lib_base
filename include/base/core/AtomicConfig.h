#pragma once 

#ifndef _SILENCE_ALL_CXX20_DEPRECATION_WARNINGS
	#define _SILENCE_ALL_CXX20_DEPRECATION_WARNINGS
#endif // _SILENCE_ALL_CXX20_DEPRECATION_WARNINGS

#ifdef __STDC_NO_ATOMICS__ 
	#undef __STDC_NO_ATOMICS__ 
#endif // __STDC_NO_ATOMICS__ 

#include <atomic>
#include <type_traits>

#include <base/system/Platform.h>

namespace base {

	template <int N>
	struct AtomicTraits
	{
		static inline bool isLockFree();
	};

	template<int Size>
	struct AtomicOpsSupport
	{
		enum {
			IsSupported = (Size == sizeof(int) || Size == sizeof(ptrdiff))
		};
	};

	template <typename T> struct AtomicAdditiveType
	{
		typedef T AdditiveT;
		static const int AddScale = 1;
	};

	template <typename T> struct AtomicAdditiveType<T*>
	{
		typedef ptrdiff AdditiveT;
		static const int AddScale = sizeof(T);
	};


#define ATOMIC_INT32_IS_SUPPORTED
#if ATOMIC_INT_LOCK_FREE == 2
#  define ATOMIC_INT_REFERENCE_COUNTING_IS_ALWAYS_NATIVE
#  define ATOMIC_INT_TEST_AND_SET_IS_ALWAYS_NATIVE

#  define ATOMIC_INT_FETCH_AND_STORE_IS_ALWAYS_NATIVE
#  define ATOMIC_INT_FETCH_AND_ADD_IS_ALWAYS_NATIVE

#  define ATOMIC_INT32_REFERENCE_COUNTING_IS_ALWAYS_NATIVE
#  define ATOMIC_INT32_TEST_AND_SET_IS_ALWAYS_NATIVE

#  define ATOMIC_INT32_FETCH_AND_STORE_IS_ALWAYS_NATIVE
#  define ATOMIC_INT32_FETCH_AND_ADD_IS_ALWAYS_NATIVE

	template <> inline bool AtomicTraits<4>::isLockFree()
	{
		return true;
	}
#elif ATOMIC_INT_LOCK_FREE == 1
#  define ATOMIC_INT_REFERENCE_COUNTING_IS_SOMETIMES_NATIVE
#  define ATOMIC_INT_TEST_AND_SET_IS_SOMETIMES_NATIVE

#  define ATOMIC_INT_FETCH_AND_STORE_IS_SOMETIMES_NATIVE
#  define ATOMIC_INT_FETCH_AND_ADD_IS_SOMETIMES_NATIVE

#  define ATOMIC_INT32_REFERENCE_COUNTING_IS_SOMETIMES_NATIVE
#  define ATOMIC_INT32_TEST_AND_SET_IS_SOMETIMES_NATIVE

#  define ATOMIC_INT32_FETCH_AND_STORE_IS_SOMETIMES_NATIVE
#  define ATOMIC_INT32_FETCH_AND_ADD_IS_SOMETIMES_NATIVE

	template <> inline bool AtomicTraits<4>::isLockFree()
	{
		return false;
	}
#else
#  define ATOMIC_INT_REFERENCE_COUNTING_IS_NEVER_NATIVE
#  define ATOMIC_INT_TEST_AND_SET_IS_NEVER_NATIVE

#  define ATOMIC_INT_FETCH_AND_STORE_IS_NEVER_NATIVE
#  define ATOMIC_INT_FETCH_AND_ADD_IS_NEVER_NATIVE

#  define ATOMIC_INT32_REFERENCE_COUNTING_IS_NEVER_NATIVE
#  define ATOMIC_INT32_TEST_AND_SET_IS_NEVER_NATIVE

#  define ATOMIC_INT32_FETCH_AND_STORE_IS_NEVER_NATIVE
#  define ATOMIC_INT32_FETCH_AND_ADD_IS_NEVER_NATIVE

	template <> inline bool AtomicTraits<4>::isLockFree()
	{
		return false;
	}
#endif

#if ATOMIC_POINTER_LOCK_FREE == 2

#  define ATOMIC_POINTER_REFERENCE_COUNTING_IS_ALWAYS_NATIVE
#  define ATOMIC_POINTER_TEST_AND_SET_IS_ALWAYS_NATIVE

#  define ATOMIC_POINTER_FETCH_AND_STORE_IS_ALWAYS_NATIVE
#  define ATOMIC_POINTER_FETCH_AND_ADD_IS_ALWAYS_NATIVE

#elif ATOMIC_POINTER_LOCK_FREE == 1

#  define ATOMIC_POINTER_REFERENCE_COUNTING_IS_SOMETIMES_NATIVE
#  define ATOMIC_POINTER_TEST_AND_SET_IS_SOMETIMES_NATIVE

#  define ATOMIC_POINTER_FETCH_AND_STORE_IS_SOMETIMES_NATIVE
#  define ATOMIC_POINTER_FETCH_AND_ADD_IS_SOMETIMES_NATIVE

#else

#  define ATOMIC_POINTER_REFERENCE_COUNTING_IS_NEVER_NATIVE
#  define ATOMIC_POINTER_TEST_AND_SET_IS_NEVER_NATIVE

#  define ATOMIC_POINTER_FETCH_AND_STORE_IS_NEVER_NATIVE
#  define ATOMIC_POINTER_FETCH_AND_ADD_IS_NEVER_NATIVE

#endif

	template<> struct AtomicOpsSupport<1> { enum { IsSupported = 1 }; };
#define ATOMIC_INT8_IS_SUPPORTED
#if ATOMIC_CHAR_LOCK_FREE == 2

#  define ATOMIC_INT8_REFERENCE_COUNTING_IS_ALWAYS_NATIVE
#  define ATOMIC_INT8_TEST_AND_SET_IS_ALWAYS_NATIVE

#  define ATOMIC_INT8_FETCH_AND_STORE_IS_ALWAYS_NATIVE
#  define ATOMIC_INT8_FETCH_AND_ADD_IS_ALWAYS_NATIVE

	template <> inline bool AtomicTraits<1>::isLockFree()
	{
		return true;
	}
#elif ATOMIC_CHAR_LOCK_FREE == 1

#  define ATOMIC_INT8_REFERENCE_COUNTING_IS_SOMETIMES_NATIVE
#  define ATOMIC_INT8_TEST_AND_SET_IS_SOMETIMES_NATIVE

#  define ATOMIC_INT8_FETCH_AND_STORE_IS_SOMETIMES_NATIVE
#  define ATOMIC_INT8_FETCH_AND_ADD_IS_SOMETIMES_NATIVE

	template <> inline bool AtomicTraits<1>::isLockFree()
	{
		return false;
	}
#else

#  define ATOMIC_INT8_REFERENCE_COUNTING_IS_NEVER_NATIVE
#  define ATOMIC_INT8_TEST_AND_SET_IS_NEVER_NATIVE

#  define ATOMIC_INT8_FETCH_AND_STORE_IS_NEVER_NATIVE
#  define ATOMIC_INT8_FETCH_AND_ADD_IS_NEVER_NATIVE

	template <> bool AtomicTraits<1>::isLockFree()
	{
		return false;
	}
#endif

	template<> struct AtomicOpsSupport<2> { enum { IsSupported = 1 }; };
#define ATOMIC_INT16_IS_SUPPORTED
#if ATOMIC_SHORT_LOCK_FREE == 2

#  define ATOMIC_INT16_REFERENCE_COUNTING_IS_ALWAYS_NATIVE
#  define ATOMIC_INT16_TEST_AND_SET_IS_ALWAYS_NATIVE

#  define ATOMIC_INT16_FETCH_AND_STORE_IS_ALWAYS_NATIVE
#  define ATOMIC_INT16_FETCH_AND_ADD_IS_ALWAYS_NATIVE

	template <> inline bool AtomicTraits<2>::isLockFree()
	{
		return false;
	}
#elif ATOMIC_SHORT_LOCK_FREE == 1

#  define ATOMIC_INT16_REFERENCE_COUNTING_IS_SOMETIMES_NATIVE
#  define ATOMIC_INT16_TEST_AND_SET_IS_SOMETIMES_NATIVE

#  define ATOMIC_INT16_FETCH_AND_STORE_IS_SOMETIMES_NATIVE
#  define ATOMIC_INT16_FETCH_AND_ADD_IS_SOMETIMES_NATIVE

	template <> inline bool AtomicTraits<2>::isLockFree()
	{
		return false;
	}
#else

#  define ATOMIC_INT16_REFERENCE_COUNTING_IS_NEVER_NATIVE
#  define ATOMIC_INT16_TEST_AND_SET_IS_NEVER_NATIVE

#  define ATOMIC_INT16_FETCH_AND_STORE_IS_NEVER_NATIVE
#  define ATOMIC_INT16_FETCH_AND_ADD_IS_NEVER_NATIVE

	template <> inline bool AtomicTraits<2>::isLockFree()
	{
		return false;
	}
#endif

	template<> struct AtomicOpsSupport<8> { enum { IsSupported = 1 }; };
#  define ATOMIC_INT64_IS_SUPPORTED
#  if ATOMIC_LLONG_LOCK_FREE == 2

#    define ATOMIC_INT64_REFERENCE_COUNTING_IS_ALWAYS_NATIVE
#    define ATOMIC_INT64_TEST_AND_SET_IS_ALWAYS_NATIVE

#    define ATOMIC_INT64_FETCH_AND_STORE_IS_ALWAYS_NATIVE
#    define ATOMIC_INT64_FETCH_AND_ADD_IS_ALWAYS_NATIVE

	template <> inline bool AtomicTraits<8>::isLockFree()
	{
		return true;
	}
#  elif ATOMIC_LLONG_LOCK_FREE == 1

#    define ATOMIC_INT64_REFERENCE_COUNTING_IS_SOMETIMES_NATIVE
#    define ATOMIC_INT64_TEST_AND_SET_IS_SOMETIMES_NATIVE

#    define ATOMIC_INT64_FETCH_AND_STORE_IS_SOMETIMES_NATIVE
#    define ATOMIC_INT64_FETCH_AND_ADD_IS_SOMETIMES_NATIVE

	template <> inline bool AtomicTraits<8>::isLockFree()
	{
		return false;
	}
#  else

#    define ATOMIC_INT64_REFERENCE_COUNTING_IS_NEVER_NATIVE
#    define ATOMIC_INT64_TEST_AND_SET_IS_NEVER_NATIVE

#    define ATOMIC_INT64_FETCH_AND_STORE_IS_NEVER_NATIVE
#    define ATOMIC_INT64_FETCH_AND_ADD_IS_NEVER_NATIVE

	template <> inline bool AtomicTraits<8>::isLockFree()
	{
		return false;
	}
#  endif

} // namespace base