#pragma once 


#ifdef LIB_BASE_ENABLE_QT
#include <base/qt/common/Types.h>
#endif

#include <functional>
#include <gsl/gsl>

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
