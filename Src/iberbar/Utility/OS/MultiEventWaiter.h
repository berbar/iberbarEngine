#pragma once




#ifdef _WINDOWS
#include <iberbar/Utility/OS/Windows/MultiEventWaiter.h>
#endif

#ifdef __ANDROID__
#include <iberbar/Utility/OS/Unix/MultiEventWaiter.h>
#endif


