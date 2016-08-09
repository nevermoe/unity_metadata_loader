#pragma once

#include <stdint.h>
#include "il2cpp-config.h"
#include "object-internals.h"

struct Il2CppString;

namespace il2cpp
{
namespace icalls
{
namespace System
{
namespace System
{
namespace Threading
{
	class LIBIL2CPP_CODEGEN_API Semaphore
	{
	public:
		static Il2CppIntPtr CreateSemaphore_internal(int32_t initialCount, int32_t maximumCount, Il2CppString* name, bool* created);
		static int32_t ReleaseSemaphore_internal(Il2CppIntPtr handlePtr, int32_t releaseCount, bool* fail);
		static Il2CppIntPtr OpenSemaphore_internal(Il2CppString* name, int32_t rights, int32_t* error);
	};

} /* namespace Threading */
} /* namespace System */
} /* namespace System */
} /* namespace icalls */
} /* namespace il2cpp */
