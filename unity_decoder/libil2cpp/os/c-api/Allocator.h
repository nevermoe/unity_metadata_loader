#pragma once

#include "il2cpp-config.h"

typedef void* (*allocate_func)(size_t size);

#if defined(__cplusplus)
extern "C"
{
#endif

void register_allocator(allocate_func allocator);

#if defined(__cplusplus)
}
#endif

#if defined(__cplusplus)
#include <string>

class Allocator
{
public:
    static void* Allocate(size_t size);
    static char* CopyToAllocatedStringBuffer(const std::string& input);
};

#endif
