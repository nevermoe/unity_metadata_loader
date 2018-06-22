#include "os/Path.h"
#include "Allocator.h"

#include <string>

extern "C"
{
const char* UnityPalGetTempPath()
{
    return Allocator::CopyToAllocatedStringBuffer(il2cpp::os::Path::GetTempPath());
}

const char* UnityPalGetExecutablePath()
{
    return Allocator::CopyToAllocatedStringBuffer(il2cpp::os::Path::GetExecutablePath());
}
}
