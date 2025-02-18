#pragma once

#include <base/system/SystemConfig.h>

#include <base/core/Types.h>
#include <base/core/BaseExport.h>

#include <base/utility/CommonMacros.h>

#include <cstdint>
#include <cstdio>

#include <span>

#define SUCCESS                             0
/** Unspecified error */
#define EGENERIC                            (-2 * (1 << (sizeof (int) * 8 - 2))) /* INT_MIN */

#define unused(x)								((void)(x))