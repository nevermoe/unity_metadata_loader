#include "il2cpp-config.h"
#include <cassert>
#include "icalls/mscorlib/System.Threading/NativeEventCalls.h"
#include "os/Event.h"
#include "vm/Exception.h"

using namespace il2cpp::os;

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

bool NativeEventCalls::ResetEvent_internal (Il2CppIntPtr handlePtr)
{
	EventHandle* handle = (EventHandle*)handlePtr.m_value;
	ErrorCode result = handle->Get ().Reset ();

	return kErrorCodeSuccess == result;
}

bool NativeEventCalls::SetEvent_internal (Il2CppIntPtr handlePtr)
{
	EventHandle* handle = (EventHandle*)handlePtr.m_value;
	ErrorCode result = handle->Get ().Set ();

	return kErrorCodeSuccess == result;
}

Il2CppIntPtr NativeEventCalls::CreateEvent_internal (bool manualReset, bool signaled, Il2CppString* name, bool* created)
{
	*created = true;
	il2cpp::os::Event* event = NULL;
	
	if (name == NULL)
		event = new Event (manualReset, signaled);
	else 
		NOT_SUPPORTED_IL2CPP (NativeEventCalls::CreateEvent_internal, "Named events are not supported.");

	Il2CppIntPtr ret = { new EventHandle(event) };
	return ret;
}

void NativeEventCalls::CloseEvent_internal (Il2CppIntPtr handlePtr)
{
	Handle* handle = (Handle*)handlePtr.m_value;
	// should we close or just delete
	//handle->Close ();
	delete handle;
}

Il2CppIntPtr NativeEventCalls::OpenEvent_internal (Il2CppString* name, EventWaitHandleRights rights, MonoIOError* error)
{
	NOT_IMPLEMENTED_ICALL (NativeEventCalls::OpenEvent_internal);
	
	return Il2CppIntPtr();
}

} /* namespace Threading */
} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
