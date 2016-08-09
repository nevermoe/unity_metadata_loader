#pragma once

#if IL2CPP_PLATFORM_WIN32

#define WIN32_LEAN_AND_MEAN 1
#define INC_OLE2 1
#include <Windows.h>

#if IL2CPP_TARGET_WINRT || IL2CPP_TARGET_XBOX
#include <roapi.h>
#include <winstring.h>
#endif

#define LINK_TO_WINDOWSRUNTIME_LIBS (IL2CPP_TARGET_WINRT || IL2CPP_TARGET_XBOX)

#endif
