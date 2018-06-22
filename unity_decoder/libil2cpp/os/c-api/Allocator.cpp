#include "Allocator.h"

static allocate_func s_Allocator;

extern "C"
{
void register_allocator(allocate_func allocator)
{
    s_Allocator = allocator;
}
}

void* Allocator::Allocate(size_t size)
{
    IL2CPP_ASSERT(s_Allocator);
    return s_Allocator(size);
}

char* Allocator::CopyToAllocatedStringBuffer(const std::string& input)
{
    size_t size = input.size();
    char* buffer = (char*)Allocator::Allocate(size + 1);
    input.copy(buffer, size);
    buffer[size] = '\0';
    return buffer;
}
