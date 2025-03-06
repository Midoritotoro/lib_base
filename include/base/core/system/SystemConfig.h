#pragma once

#include <base/core/system/SystemString.h>

#include <base/core/arch/SimdHelpers.h>
#include <base/core/utility/Assert.h>

#include <base/core/utility/Flags.h>
#include <base/core/arch/StaticCodeAnalysis.h>

#ifdef LIB_BASE_SYSTEM_NO_FAILURE
    #define SystemAssert						AssertReturn
#else
    #define SystemAssert(cond, mes, retval)		AssertLog(cond, mes)
#endif

