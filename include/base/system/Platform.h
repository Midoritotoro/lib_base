#pragma once


#include <base/Types.h>
#include <base/BaseExport.h>

#include <base/system/SystemConfig.h>

#include <cstdint>
#include <cstdio>

#include <span>

#define SUCCESS                             0
/** Unspecified error */
#define EGENERIC                            (-2 * (1 << (sizeof (int) * 8 - 2))) /* INT_MIN */

#define unused(x)								((void)(x))