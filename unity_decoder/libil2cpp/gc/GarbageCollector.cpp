#include "il2cpp-config.h"
#include "object-internals.h"
#include "GarbageCollector.h"
#include "vm/Class.h"
#include "vm/Domain.h"
#include "vm/Exception.h"
#include "vm/Runtime.h"
#include "vm/Thread.h"
#include "os/Event.h"
#include "os/Semaphore.h"
#include "os/Thread.h"
#include <cassert>

using namespace il2cpp::os;
using namespace il2cpp::vm;

#if IL2CPP_SUPPORT_THREADS

static bool s_StopFinalizer = false;
static il2cpp::os::Thread s_FinalizerThread;
static Il2CppThread* s_FinalizerThreadObject;
static Semaphore s_FinalizerSemaphore;
static Event s_FinalizersThreadStartedEvent;
static Event s_FinalizersCompletedEvent(true, false);

static void FinalizerThread (void* arg)
{
	s_FinalizerThreadObject = il2cpp::vm::Thread::Attach (Domain::GetCurrent ());
	s_FinalizerThread.SetName ("GC Finalizer");

	s_FinalizersThreadStartedEvent.Set();

	while (!s_StopFinalizer)
	{
		s_FinalizerSemaphore.Wait ();

		il2cpp::gc::GarbageCollector::InvokeFinalizers ();

		s_FinalizersCompletedEvent.Set ();
	}

	il2cpp::vm::Thread::Detach (s_FinalizerThreadObject);
}

bool il2cpp::gc::GarbageCollector::IsFinalizerThread (Il2CppThread *thread)
{
	return s_FinalizerThreadObject == thread;
}

#else

bool il2cpp::gc::GarbageCollector::IsFinalizerThread (Il2CppThread *thread)
{
	return false;
}

#endif

void
il2cpp::gc::GarbageCollector::InitializeFinalizer ()
{
	GarbageCollector::InvokeFinalizers ();
#if IL2CPP_SUPPORT_THREADS
	s_FinalizerThread.Run (&FinalizerThread, NULL);
	s_FinalizersThreadStartedEvent.Wait();
#endif
}


void
il2cpp::gc::GarbageCollector::Uninitialize ()
{
#if IL2CPP_SUPPORT_THREADS
	s_StopFinalizer = true;
	NotifyFinalizers ();
	s_FinalizerThread.Join ();
#endif
}

void
il2cpp::gc::GarbageCollector::NotifyFinalizers ()
{
#if IL2CPP_SUPPORT_THREADS
	s_FinalizerSemaphore.Post (1, NULL);
#endif
}

void
il2cpp::gc::GarbageCollector::RunFinalizer (void *obj, void *data)
{
	NOT_IMPLEMENTED_NO_ASSERT (il2cpp::gc::GarbageCollector::RunFinalizer, "Compare to mono implementation special cases");

	Il2CppException *exc = NULL;
	Il2CppObject *o;
	const MethodInfo* finalizer = NULL;

	o = (Il2CppObject*)obj;

	finalizer = Class::GetFinalizer (o->klass);

	Runtime::Invoke (finalizer, o, NULL, &exc);

	if (exc)
		Runtime::UnhandledException (exc);
}

void il2cpp::gc::GarbageCollector::RegisterFinalizer (Il2CppObject* obj)
{
	RegisterFinalizerWithCallback (obj, &il2cpp::gc::GarbageCollector::RunFinalizer);
}

void il2cpp::gc::GarbageCollector::SuppressFinalizer (Il2CppObject* obj)
{
	RegisterFinalizerWithCallback (obj, NULL);
}

void il2cpp::gc::GarbageCollector::WaitForPendingFinalizers ()
{
	if (!il2cpp::gc::GarbageCollector::HasPendingFinalizers ())
		return;
	
#if IL2CPP_SUPPORT_THREADS
	/* Avoid deadlocks */
	if (il2cpp::vm::Thread::Current () == s_FinalizerThreadObject)
		return;

	s_FinalizersCompletedEvent.Reset ();
	NotifyFinalizers ();
	s_FinalizersCompletedEvent.Wait ();
#else
	il2cpp::gc::GarbageCollector::InvokeFinalizers ();
#endif
}

int32_t
il2cpp::gc::GarbageCollector::GetGeneration (void* addr)
{
	return 0;
}

void
il2cpp::gc::GarbageCollector::AddMemoryPressure (int64_t value)
{
}

