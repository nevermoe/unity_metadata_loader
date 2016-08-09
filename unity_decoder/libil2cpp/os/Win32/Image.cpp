#include "il2cpp-config.h"

#if IL2CPP_PLATFORM_WIN32

#include "WindowsHeaders.h"

EXTERN_C IMAGE_DOS_HEADER __ImageBase;

namespace il2cpp
{
namespace os
{

namespace Image
{

void* GetImageBase ()
{
	return &__ImageBase;
}

}
}
}

#endif
