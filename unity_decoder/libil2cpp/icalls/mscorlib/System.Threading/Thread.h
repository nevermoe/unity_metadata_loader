#pragma once

#include <stdint.h>
#include "object-internals.h"
#include "vm/Thread.h"
#include "il2cpp-config.h"

struct Il2CppString;
struct Il2CppThread;
struct mscorlib_System_Globalization_CultureInfo;
struct Il2CppDelegate;
struct mscorlib_System_Threading_Thread;

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

class LIBIL2CPP_CODEGEN_API Thread
{
public:
	static int32_t GetDomainID ();
	static Il2CppThread * CurrentThread_internal ();
	static void ResetAbort_internal ();
	static void MemoryBarrier_ ();
	static void SpinWait_nop ();
	static void Abort_internal (Il2CppThread* __this, Il2CppObject* stateInfo);
	static void ClrState (Il2CppThread* __this, il2cpp::vm::ThreadState clr);
	static void FreeLocalSlotValues (int32_t slot, bool use_thread_local);
	static Il2CppObject* GetAbortExceptionState (void* /* System.Threading.Thread */ self);
	static mscorlib_System_Globalization_CultureInfo * GetCachedCurrentCulture (Il2CppThread* __this);
	static mscorlib_System_Globalization_CultureInfo* GetCachedCurrentUICulture (Il2CppThread* __this);
	static Il2CppString* GetName_internal (Il2CppThread* __this);
	static void SetName_internal (Il2CppThread* __this, Il2CppString* name);
	static int32_t GetNewManagedId_internal();
	static Il2CppArray * GetSerializedCurrentCulture (Il2CppThread* __this);
	static Il2CppArray* GetSerializedCurrentUICulture (Il2CppThread* __this);
	static il2cpp::vm::ThreadState GetState (Il2CppThread * __this);
	static void Interrupt_internal (Il2CppThread* __this);
	static bool Join_internal (Il2CppThread * __this, int32_t ms, void* thread);
	static void Resume_internal (void* /* System.Threading.Thread */ self);
	static void SetCachedCurrentCulture (Il2CppThread *__this,Il2CppObject* culture);
	static void SetCachedCurrentUICulture (Il2CppThread* __this, Il2CppObject* culture);
	static void SetSerializedCurrentCulture (Il2CppThread* __this, Il2CppArray* culture);
	static void SetSerializedCurrentUICulture (Il2CppThread* __this, Il2CppArray* culture);
	static void SetState (Il2CppThread * __this, il2cpp::vm::ThreadState state);
	static void Sleep_internal (int32_t milliseconds);
	static void Suspend_internal (void* /* System.Threading.Thread */ self);
	static void Thread_init (Il2CppThread* __this);
	static Il2CppIntPtr Thread_internal (Il2CppThread* __this, Il2CppDelegate * start);
	static int8_t VolatileReadInt8 (volatile void* address);
	static int16_t VolatileReadInt16 (volatile void* address);
	static int32_t VolatileReadInt32 (volatile void* address);
	static int64_t VolatileReadInt64 (volatile void* address);
	static float VolatileReadFloat (volatile void* address);
	static double VolatileReadDouble (volatile void* address);
	static void* VolatileReadPtr (volatile void* address);
	static Il2CppIntPtr VolatileReadIntPtr(volatile void* address);
	static void VolatileWriteInt8 (volatile void* address, int8_t value);
	static void VolatileWriteInt16 (volatile void* address, int16_t value);
	static void VolatileWriteInt32 (volatile void* address, int32_t value);
	static void VolatileWriteInt64 (volatile void* address, int64_t value);
	static void VolatileWriteFloat (volatile void* address, float value);
	static void VolatileWriteDouble (volatile void* address, double value);
	static void VolatileWritePtr (volatile void* address, void* value);
	static void VolatileWriteIntPtr (volatile void* address, Il2CppIntPtr value);
	static void Thread_free_internal (Il2CppThread* __this, Il2CppIntPtr handle);
};

} /* namespace Threading */
} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
