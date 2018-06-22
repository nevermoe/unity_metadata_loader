#pragma once

#if IL2CPP_TARGET_WINDOWS

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN 1
#define INC_OLE2 1
#include <Windows.h>

#if !IL2CPP_TARGET_WINDOWS_DESKTOP
#include <bcrypt.h>
#include <roapi.h>
#include <robuffer.h>
#include <winstring.h>
#endif

#define LINK_TO_WINDOWSRUNTIME_LIBS (!IL2CPP_TARGET_WINDOWS_DESKTOP)

#endif
