#pragma once

#include "concurrent/common/CommonConfig.h"

#include "concurrent/common/CommonQueue.h"
#include "concurrent/winapi/WinapiSemaphore.h"

#include "concurrent/common/CommonAsyncGuarded.h"


#if defined USE_WINAPI
#include "concurrent/winapi/WinapiAsync.h"
#else // USE_WINAPI
#error "Configuration is not supported."
#endif // !USE_WINAPI