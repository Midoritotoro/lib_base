#pragma once 

#include <base/core/utility/Assert.h>

#ifndef _STRING_OUT_OF_RANGE_
#  define _STRING_OUT_OF_RANGE_ ("base::string::BasicString: Index out of range.\n ")
#endif

#if defined(_DEBUG)

#ifdef BASE_STRING_DEBUG_ASSERT_NO_FAILURE
#  define _STRING_DEBUG_ASSERT_LOG_(_Cond, _RetVal, _Message)					\
    DebugAssertReturn(_Cond, _Message, _RetVal)
#else
#  define _STRING_DEBUG_ASSERT_LOG_(_Cond, _RetVal, _Message)					\
	UNUSED(_RetVal);															\
	DebugAssert(_Cond, _Message)												
#endif

#  ifndef _STRING_ERROR_DEBUG_
#    define _STRING_ERROR_DEBUG_(_Messsage, _RetVal)							\
	do {																		\
		_STRING_DEBUG_ASSERT_LOG_(false, _Messsage);							\
		return _RetVal;															\
	} while(0);
#  endif

#  ifndef _STRING_ERROR_DEBUG_NO_RET_
#    define _STRING_ERROR_DEBUG_NO_RET_(_Messsage)								\
	do {																		\
		_STRING_DEBUG_ASSERT_LOG_(false, _Messsage);							\
		return;																	\
	} while(0);
#  endif

#  ifndef _STRING_NOT_ENOUGH_MEMORY_DEBUG_
#    define _STRING_NOT_ENOUGH_MEMORY_DEBUG_(_RetVal)							\
    _STRING_ERROR_DEBUG_(														\
      "base::string::BasicString: Not enough memory to expand the Vector.\n ",	\
        _RetVal)
#  endif

#  ifndef _STRING_TOO_LONG_DEBUG_
#    define _STRING_TOO_LONG_DEBUG_(_RetVal)									\
    _STRING_ERROR_DEBUG_(														\
      "base::string::BasicString: Vector too long.\n ",							\
        _RetVal)
#  endif

#  ifndef _STRING_INDEX_OUT_OF_RANGE_DEBUG_
#    define _STRING_INDEX_OUT_OF_RANGE_DEBUG_									\
    _STRING_ERROR_DEBUG_NO_RET_(												\
      _STRING_OUT_OF_RANGE_)
#  endif

#  ifndef _STRING_NOT_ENOUGH_MEMORY_DEBUG_NO_RET_
#    define _STRING_NOT_ENOUGH_MEMORY_DEBUG_NO_RET_								\
    _STRING_ERROR_DEBUG_NO_RET_(												\
      "base::string::BasicString: Not enough memory to expand the Vector.\n ")
#  endif

#  ifndef _STRING_TOO_LONG_DEBUG_NO_RET_
#    define _STRING_TOO_LONG_DEBUG_NO_RET_										\
    _STRING_ERROR_DEBUG_NO_RET_(												\
      "base::string::BasicString: Vector too long.\n ")
#  endif

#  ifndef _STRING_INDEX_OUT_OF_RANGE_DEBUG_NO_RET_
#    define _STRING_INDEX_OUT_OF_RANGE_DEBUG_NO_RET_							\
    _STRING_ERROR_DEBUG_NO_RET_(												\
      _STRING_OUT_OF_RANGE_)
#  endif

#else

#  ifndef _STRING_ERROR_DEBUG_
#    define _STRING_ERROR_DEBUG_(_Messsage, _RetVal)							((void)0)
#  endif

#  ifndef _STRING_ERROR_DEBUG_NO_RET_
#    define _STRING_ERROR_DEBUG_NO_RET_(_Message)								((void)0)
#  endif

#  ifndef _STRING_NOT_ENOUGH_MEMORY_DEBUG_
#    define _STRING_NOT_ENOUGH_MEMORY_DEBUG_(_RetVal)							((void)0)
#  endif

#  ifndef _STRING_TOO_LONG_DEBUG_
#    define _STRING_TOO_LONG_DEBUG_(_RetVal)									((void)0)
#  endif

#  ifndef _STRING_INDEX_OUT_OF_RANGE_DEBUG_
#    define _STRING_INDEX_OUT_OF_RANGE_DEBUG_									((void)0)
#  endif

#  ifndef _STRING_NOT_ENOUGH_MEMORY_DEBUG_NO_RET_
#    define _STRING_NOT_ENOUGH_MEMORY_DEBUG_NO_RET_								((void)0)
#  endif

#  ifndef _STRING_TOO_LONG_DEBUG_NO_RET_
#    define _STRING_TOO_LONG_DEBUG_NO_RET_										((void)0)
#  endif

#  ifndef _STRING_INDEX_OUT_OF_RANGE_DEBUG_NO_RET_
#    define _STRING_INDEX_OUT_OF_RANGE_DEBUG_NO_RET_							((void)0)
#  endif

#  ifndef _STRING_DEBUG_ASSERT_LOG_
#    define _STRING_DEBUG_ASSERT_LOG_											((void)0)
#endif

#endif

#ifndef BASE_SILENCE_ALL_STRING_WARNINGS

#  ifndef _STRING_PUSH_FRONT_INEFFICIENT_WARNING_
#    define _STRING_PUSH_FRONT_INEFFICIENT_WARNING_ \
	BASE_DEPRECATED_WARNING(						\
		"base::string::BasicString::pushFront: Using the pushFront method is inefficient in terms of performance")
#endif

#else

#  ifndef _STRING_PUSH_FRONT_INEFFICIENT_WARNING_
#    define _STRING_PUSH_FRONT_INEFFICIENT_WARNING_
#endif

#endif