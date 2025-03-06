#pragma once

#include <base/core/async/CommonConfig.h>

#include <base/core/async/CommonQueue.h>
#include <base/core/async/WinapiSemaphore.h>

#include <base/core/async/CommonAsyncGuarded.h>


#if defined USE_WINAPI
#include <base/core/async/WinapiAsync.h>
#else // USE_WINAPI
#error "Configuration is not supported."
#endif // !USE_WINAPI