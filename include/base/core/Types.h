#pragma once 

#ifdef LIB_BASE_ENABLE_QT
    #include <base/qt/common/Types.h>
#endif

#include <base/core/arch/ProcessorDetection.h>
#include <base/core/BaseNamespace.h>

#include <base/core/arch/KeywordSupport.h>

#include <functional>
#include <base/core/utility/NotNull.h>

#include <numeric>

__BASE_NAMESPACE_BEGIN

template <typename Signature>
using Fn = std::function<Signature>;

using uchar     = unsigned char;
using ushort    = unsigned short;

using uint      = unsigned int;
using ulong     = unsigned long;

using int8      = signed char;			
using uint8     = unsigned char;		

using int16     = short;			
using uint16    = unsigned short;		

using int32     = int;					
using uint32    = unsigned int;		

using int64     = long long;		
using uint64    = unsigned long long;

using longlong  = int64;
using ulonglong = uint64;

using ulong32   = unsigned long;
using long32    = long;

template <int>      struct IntegerForSize;

template <>         struct IntegerForSize<1> { typedef uint8  Unsigned; typedef int8  Signed; };
template <>         struct IntegerForSize<2> { typedef uint16 Unsigned; typedef int16 Signed; };

template <>         struct IntegerForSize<4> { typedef uint32 Unsigned; typedef int32 Signed; };
template <>         struct IntegerForSize<8> { typedef uint64 Unsigned; typedef int64 Signed; };

template <class T>  struct IntegerForSizeof : IntegerForSize<sizeof(T)> { };

using registerint   = IntegerForSize<PROCESSOR_WORDSIZE>::Signed;
using registeruint  = IntegerForSize<PROCESSOR_WORDSIZE>::Unsigned;

using uintptr       = IntegerForSizeof<void*>::Unsigned;
using ptrdiff       = IntegerForSizeof<void*>::Signed;

using intptr        = ptrdiff;
using sizetype      = IntegerForSizeof<std::size_t>::Unsigned;

using byte_t    = uint8;
using sbyte_t   = int8;

using word_t    = uint16;
using sword_t   = int16;

using dword_t   = ulong32;
using sdword_t  = long32;

using qword_t   = uint64;
using sqword_t  = int64;

#if defined(OS_WIN)
    using bool_t = int;
#else
    using bool_t = bool;
#endif

__BASE_NAMESPACE_END


#define INT64_CAST(c)       static_cast<long long>(c ## LL) 
#define UINT64_CAST(c)      static_cast<unsigned long long>(c ## ULL)

#define BASE_INT8_MAX       (std::numeric_limits<int8_t>::max())
#define BASE_INT16_MAX      (std::numeric_limits<int16_t>::max())
#define BASE_INT32_MAX      (std::numeric_limits<int32_t>::max())
#define BASE_INT64_MAX      (std::numeric_limits<int64_t>::max())

#define BASE_UINT8_MAX      (std::numeric_limits<uint8_t>::max())
#define BASE_UINT16_MAX     (std::numeric_limits<uint16_t>::max())
#define BASE_UINT32_MAX     (std::numeric_limits<uint32_t>::max())
#define BASE_UINT64_MAX     (std::numeric_limits<uint64_t>::max())

#define BASE_INT8_MIN       (std::numeric_limits<int8_t>::min())
#define BASE_INT16_MIN      (std::numeric_limits<int16_t>::min())
#define BASE_INT32_MIN      (std::numeric_limits<int32_t>::min())
#define BASE_INT64_MIN      (std::numeric_limits<int64_t>::min())
    
#define BASE_UINT8_MIN      (0)
#define BASE_UINT16_MIN     (0)
#define BASE_UINT32_MIN     (0)
#define BASE_UINT64_MIN     (0)