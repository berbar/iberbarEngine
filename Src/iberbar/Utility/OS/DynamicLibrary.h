#pragma once

#ifdef _WINDOWS
#include <iberbar/Utility/OS/Windows/DynamicLibrary.h>
#endif

#ifdef __ANDROID__
#include <iberbar/Utility/OS/Unix/DynamicLibrary.h>
#endif
