#pragma once 

#ifdef LIB_BASE_ENABLE_QT
    #include <base/qt/common/Types.h>
#endif

#include <base/system/ProcessorDetection.h>

#include <functional>
#include <gsl/gsl>

#include <numeric>

using gsl::not_null;

template <typename Signature>
using Fn = std::function<Signature>;



using uchar		= unsigned char;
using ushort	= unsigned short;

using uint		= unsigned int;
using ulong		= unsigned long;

using int8		= signed char;			/* 8 bit signed */
using uint8		= unsigned char;		/* 8 bit unsigned */

using int16		= short;				/* 16 bit signed */
using uint16	= unsigned short;		/* 16 bit unsigned */

using int32		= int;					/* 32 bit signed */
using uint32	= unsigned int;			/* 32 bit unsigned */

using int64		= long long;			/* 64 bit signed */
using uint64	= unsigned long long;	/* 64 bit unsigned */

using longlong  = int64;
using ulonglong = uint64;


template <int>      struct IntegerForSize;

template <>         struct IntegerForSize<1> { typedef uint8  Unsigned; typedef int8  Signed; };
template <>         struct IntegerForSize<2> { typedef uint16 Unsigned; typedef int16 Signed; };

template <>         struct IntegerForSize<4> { typedef uint32 Unsigned; typedef int32 Signed; };
template <>         struct IntegerForSize<8> { typedef uint64 Unsigned; typedef int64 Signed; };

template <class T>  struct IntegerForSizeof : IntegerForSize<sizeof(T)> { };

typedef IntegerForSize<PROCESSOR_WORDSIZE>::Signed registerint;
typedef IntegerForSize<PROCESSOR_WORDSIZE>::Unsigned registeruint;

typedef IntegerForSizeof<void*>::Unsigned uintptr;
typedef IntegerForSizeof<void*>::Signed ptrdiff;

typedef ptrdiff qintptr;

using sizetype = IntegerForSizeof<std::size_t>::Signed;

#  define INT64_CAST(c) static_cast<long long>(c ## LL)     /* signed 64 bit constant */
#  define UINT64_CAST(c) static_cast<unsigned long long>(c ## ULL) /* unsigned 64 bit constant */