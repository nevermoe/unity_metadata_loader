#pragma once

#include <stdint.h>

#include "il2cpp-config.h"

#if IL2CPP_API_DYNAMIC_NO_DLSYM
extern "C"
{
	IL2CPP_EXPORT void il2cpp_api_register_symbols(void);
	IL2CPP_EXPORT void* il2cpp_api_lookup_symbol(const char* name);
}
#endif

extern "C"
{
#define DO_API(r,n,p)	IL2CPP_EXPORT r n p;
#include "il2cpp-api-functions.h"
#undef DO_API
}
