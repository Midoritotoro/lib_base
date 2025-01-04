#pragma once

#include <base/concurrent/common/CommonConfig.h>

#include <base/concurrent/common/CommonQueue.h>
#include <base/concurrent/win/WinapiSemaphore.h>

#include <base/concurrent/common/CommonAsyncGuarded.h>


#if defined USE_WINAPI
#include <base/concurrent/win/WinapiAsync.h>
#else // USE_WINAPI
#error "Configuration is not supported."
#endif // !USE_WINAPI