#if defined(PLATFORM_WIN32)
#include "../../src/win32/RecursiveMutex.h"
#elif defined(PLATFORM_LINUX)
#include "../../src/linux/RecursiveMutex.h"
#else
#error unsupported platform
#endif
