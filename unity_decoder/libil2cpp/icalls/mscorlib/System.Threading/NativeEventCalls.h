#pragma once

#include <stdint.h>
#include "object-internals.h"
#include "il2cpp-config.h"

struct Il2CppString;

namespace il2cpp
{
namespace icalls
{
namespace mscorlib
{
namespace System
{
namespace Threading
{

struct MonoIOError;

typedef int32_t EventWaitHandleRights;

class LIBIL2CPP_CODEGEN_API NativeEventCalls
{
public:
	static Il2CppIntPtr CreateEvent_internal (bool manualReset, bool signaled, Il2CppString* name, bool* created);
	static Il2CppIntPtr OpenEvent_internal (Il2CppString* name, EventWaitHandleRights rights, MonoIOError* error);
	static bool ResetEvent_internal (Il2CppIntPtr handlePtr);
	static bool SetEvent_internal (Il2CppIntPtr handlePtr);
	static void CloseEvent_internal (Il2CppIntPtr handlePtr);
};

} /* namespace Threading */
} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
