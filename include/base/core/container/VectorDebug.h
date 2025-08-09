#pragma once 

#include <base/core/utility/Assert.h>

#ifndef _VECTOR_OUT_OF_RANGE_
#  define _VECTOR_OUT_OF_RANGE_ ("base::container::Vector: Index out of range.\n ")
#endif

#if defined(_DEBUG)

#ifdef BASE_VECTOR_DEBUG_ASSERT_NO_FAILURE
#  define _VECTOR_DEBUG_ASSERT_LOG_(_Cond, _RetVal, _Message)					\
    DebugAssertReturn(_Cond, _Message, _RetVal)
#else
#  define _VECTOR_DEBUG_ASSERT_LOG_(_Cond, _RetVal, _Message)					\
	base_unused(_RetVal);															\
	DebugAssert(_Cond, _Message)												
#endif

#  ifndef _VECTOR_ERROR_DEBUG_
#    define _VECTOR_ERROR_DEBUG_(_Messsage, _RetVal)							\
	do {																		\
		_VECTOR_DEBUG_ASSERT_LOG_(false, _Messsage);							\
		return _RetVal;															\
	} while(0);
#  endif

#  ifndef _VECTOR_ERROR_DEBUG_NO_RET_
#    define _VECTOR_ERROR_DEBUG_NO_RET_(_Messsage)								\
	do {																		\
		_VECTOR_DEBUG_ASSERT_LOG_(false, _Messsage);							\
		return;																	\
	} while(0);
#  endif

#  ifndef _VECTOR_NOT_ENOUGH_MEMORY_DEBUG_
#    define _VECTOR_NOT_ENOUGH_MEMORY_DEBUG_(_RetVal)							\
    _VECTOR_ERROR_DEBUG_(														\
      "base::container::Vector: Not enough memory to expand the Vector.\n ",	\
        _RetVal)
#  endif

#  ifndef _VECTOR_TOO_LONG_DEBUG_
#    define _VECTOR_TOO_LONG_DEBUG_(_RetVal)									\
    _VECTOR_ERROR_DEBUG_(														\
      "base::container::Vector: Vector too long.\n ",							\
        _RetVal)
#  endif

#  ifndef _VECTOR_INDEX_OUT_OF_RANGE_DEBUG_
#    define _VECTOR_INDEX_OUT_OF_RANGE_DEBUG_									\
    _VECTOR_ERROR_DEBUG_NO_RET_(												\
      _VECTOR_OUT_OF_RANGE_)
#  endif

#  ifndef _VECTOR_NOT_ENOUGH_MEMORY_DEBUG_NO_RET_
#    define _VECTOR_NOT_ENOUGH_MEMORY_DEBUG_NO_RET_								\
    _VECTOR_ERROR_DEBUG_NO_RET_(												\
      "base::container::Vector: Not enough memory to expand the Vector.\n ")
#  endif

#  ifndef _VECTOR_TOO_LONG_DEBUG_NO_RET_
#    define _VECTOR_TOO_LONG_DEBUG_NO_RET_										\
    _VECTOR_ERROR_DEBUG_NO_RET_(												\
      "base::container::Vector: Vector too long.\n ")
#  endif

#  ifndef _VECTOR_INDEX_OUT_OF_RANGE_DEBUG_NO_RET_
#    define _VECTOR_INDEX_OUT_OF_RANGE_DEBUG_NO_RET_							\
    _VECTOR_ERROR_DEBUG_NO_RET_(												\
      _VECTOR_OUT_OF_RANGE_)
#  endif

#else

#  ifndef _VECTOR_ERROR_DEBUG_
#    define _VECTOR_ERROR_DEBUG_(_Messsage, _RetVal)							((void)0)
#  endif

#  ifndef _VECTOR_ERROR_DEBUG_NO_RET_
#    define _VECTOR_ERROR_DEBUG_NO_RET_(_Message)								((void)0)
#  endif

#  ifndef _VECTOR_NOT_ENOUGH_MEMORY_DEBUG_
#    define _VECTOR_NOT_ENOUGH_MEMORY_DEBUG_(_RetVal)							((void)0)
#  endif

#  ifndef _VECTOR_TOO_LONG_DEBUG_
#    define _VECTOR_TOO_LONG_DEBUG_(_RetVal)									((void)0)
#  endif

#  ifndef _VECTOR_INDEX_OUT_OF_RANGE_DEBUG_
#    define _VECTOR_INDEX_OUT_OF_RANGE_DEBUG_									((void)0)
#  endif

#  ifndef _VECTOR_NOT_ENOUGH_MEMORY_DEBUG_NO_RET_
#    define _VECTOR_NOT_ENOUGH_MEMORY_DEBUG_NO_RET_								((void)0)
#  endif

#  ifndef _VECTOR_TOO_LONG_DEBUG_NO_RET_
#    define _VECTOR_TOO_LONG_DEBUG_NO_RET_										((void)0)
#  endif

#  ifndef _VECTOR_INDEX_OUT_OF_RANGE_DEBUG_NO_RET_
#    define _VECTOR_INDEX_OUT_OF_RANGE_DEBUG_NO_RET_							((void)0)
#  endif

#  ifndef _VECTOR_DEBUG_ASSERT_LOG_
#    define _VECTOR_DEBUG_ASSERT_LOG_											((void)0)
#endif

#endif

#ifndef BASE_SILENCE_ALL_VECTOR_WARNINGS

#  ifndef _VECTOR_PUSH_FRONT_INEFFICIENT_WARNING_
#    define _VECTOR_PUSH_FRONT_INEFFICIENT_WARNING_ \
	base_deprecated_warning(						\
		"base::container::Vector::pushFront: Using the pushFront method is inefficient in terms of performance")
#endif

#else

#  ifndef _VECTOR_PUSH_FRONT_INEFFICIENT_WARNING_
#    define _VECTOR_PUSH_FRONT_INEFFICIENT_WARNING_
#endif

#endif