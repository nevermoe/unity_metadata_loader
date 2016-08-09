#include "il2cpp-config.h"
#include "os/Atomic.h"
#include "os/Mutex.h"
#include "os/Thread.h"
#include "os/ThreadLocalValue.h"
#include "os/StackTrace.h"
#include "vm/Domain.h"
#include "vm/Exception.h"
#include "vm/Object.h"
#include "vm/Object.h"
#include "vm/Runtime.h"
#include "vm/StackTrace.h"
#include "vm/Thread.h"
#include "gc/Allocator.h"
#include "gc/GarbageCollector.h"
#include "gc/GCHandle.h"
#include "utils/Memory.h"
#include "utils/StringUtils.h"
#include "class-internals.h"
#include "object-internals.h"
#include <cassert>
#include <algorithm>
#include <map>

#if IL2CPP_DEBUGGER_ENABLED
#include "../il2cpp-debugger.h"
#endif

using namespace il2cpp::os;
using il2cpp::gc::GarbageCollector;
using il2cpp::os::FastMutex;

namespace il2cpp
{
namespace vm
{

Il2CppThread* Thread::s_MainThread = NULL;

typedef std::vector<Il2CppThread*, il2cpp::gc::Allocator<Il2CppThread*> > GCTrackedThreadVector;

// we need to allocate this ourselves so the CRT does not initialize it and try to allocate GC memory on startup before the GC is initialized
static GCTrackedThreadVector* s_AttachedThreads;

#define AUTO_LOCK_THREADS() \
	il2cpp::os::FastAutoLock lock(&s_ThreadMutex)
static FastMutex s_ThreadMutex;

static std::vector<int32_t> s_ThreadStaticSizes;

static ThreadLocalValue s_CurrentThread;

static void
thread_cleanup_on_cancel (void* arg)
{
	Thread::Detach ((Il2CppThread*)arg);
}

void Thread::Initialize ()
{
#if IL2CPP_HAS_NATIVE_THREAD_CLEANUP
	os::Thread::SetNativeThreadCleanup(&thread_cleanup_on_cancel);
#endif
	s_AttachedThreads = new GCTrackedThreadVector ();
}

void Thread::UnInitialize ()
{
#if IL2CPP_HAS_NATIVE_THREAD_CLEANUP
	os::Thread::SetNativeThreadCleanup(NULL);
#endif
	delete s_AttachedThreads;
	s_AttachedThreads = NULL;
}

Il2CppThread* Thread::Attach (Il2CppDomain *domain)
{
	Il2CppThread* thread = Current();

	if (thread != NULL)
		return thread;

	int temp = 0;
	if (!GarbageCollector::RegisterThread (&temp))
		assert (0 && "GarbageCollector::RegisterThread failed");

	StackTrace::InitializeStackTracesForCurrentThread();

	// Get/create OS thread representing the current thread. For pre-existing threads such as
	// the main thread, this will create an OS thread instance on demand. For threads that have
	// been started through our OS layer, there will already be an instance.
	os::Thread* osThread = os::Thread::GetOrCreateCurrentThread ();

	// Create managed object representing the current thread.

	thread = (Il2CppThread*)Object::New (il2cpp_defaults.thread_class);
	thread->handle = osThread;
	thread->state = kThreadStateRunning;
	thread->tid = osThread->Id();
	Setup (thread);

	Initialize(thread, domain);

	return thread;
}

void Thread::Setup (Il2CppThread* thread)
{
	thread->synch_cs = new il2cpp::os::FastMutex ();
	thread->apartment_state = kApartmentStateUnknown;
}

void Thread::Initialize(Il2CppThread* thread, Il2CppDomain* domain)
{
	assert (thread->handle != NULL);
	assert (thread->synch_cs != NULL);

	s_CurrentThread.SetValue (thread);

	Domain::ContextSet(domain->default_context);
	Register (thread);
	AdjustStaticData ();

#if IL2CPP_DEBUGGER_ENABLED
	il2cpp_debugger_notify_thread_attach(thread);
#endif

	// Sync thread name.
	if (thread->name)
	{
		std::string utf8Name = il2cpp::utils::StringUtils::Utf16ToUtf8 (thread->name);
		thread->handle->SetName (utf8Name);
	}

	// Sync thread apartment state.
	thread->apartment_state = thread->handle->GetApartment();

#if IL2CPP_HAS_NATIVE_THREAD_CLEANUP
	// register us for platform specific cleanup attempt in case thread is not exited cleanly
	os::Thread::RegisterCurrentThreadForCleanup (thread);
#endif

	// If an interrupt has been requested before the thread was started, re-request
	// the interrupt now.
	if (thread->interruption_requested)
		RequestInterrupt (thread);
}

void Thread::Uninitialize (Il2CppThread *thread)
{
#if IL2CPP_HAS_NATIVE_THREAD_CLEANUP
	// unregister from special cleanup since we are doing it now
	os::Thread::UnregisterCurrentThreadForCleanup ();
#endif

	if (!GarbageCollector::UnregisterThread ())
		assert(0 && "GarbageCollector::UnregisterThread failed");

#if IL2CPP_DEBUGGER_ENABLED
	il2cpp_debugger_notify_thread_detach(thread);
#endif
	
	Unregister (thread);
	FreeThreadStaticData (thread);

	delete[] thread->serialized_culture_info;

	os::Thread::DetachCurrentThread ();
	s_CurrentThread.SetValue(NULL);
}

Il2CppThread* Thread::Current ()
{
	void* value = NULL;
	s_CurrentThread.GetValue (&value);
	return (Il2CppThread*)value;
}

Il2CppThread** Thread::GetAllAttachedThreads(size_t &size)
{
	size = s_AttachedThreads->size();
	return &(*s_AttachedThreads)[0];
}

static void STDCALL TerminateBackgroundThread (void* context)
{
	// We throw a dummy exception to make sure things clean up properly
	// and we don't leave any locks behind (such as global locks in the allocator which
	// would then deadlock other threads). This could work off ThreadAbortException
	// but we don't want to deal with a managed exception here. So we use a C++ exception.
	throw Thread::NativeThreadAbortException();
}

void Thread::KillAllBackgroundThreadsAndWaitForForegroundThreads ()
{
#if IL2CPP_SUPPORT_THREADS
	Il2CppThread* gcFinalizerThread = NULL;
	Il2CppThread* currentThread = Current ();
	assert (currentThread != NULL && "No current thread!");

	// Kill all threads but the finalizer and current one. We temporarily flush out
	// the entire list and then just put the two threads back.
	while (s_AttachedThreads->size ())
	{
		Il2CppThread* thread = s_AttachedThreads->back ();
		os::Thread* osThread = thread->handle;

		if (GarbageCollector::IsFinalizerThread (thread))
		{
			assert (gcFinalizerThread == NULL && "There seems to be more than one finalizer thread!");
			gcFinalizerThread = thread;
		}
		else if (thread != currentThread)
		{
			////FIXME: While we don't have stable thread abortion in place yet, work around problems in
			////	the current implementation by repeatedly requesting threads to terminate. This works around
			////    race condition to some extent.
			while (true)
			{
				// If it's a background thread, request it to kill itself.
				if (GetState (thread) & kThreadStateBackground)
				{
					////TODO: use Thread.Abort() instead
					osThread->QueueUserAPC (TerminateBackgroundThread, NULL);
				}

				// Wait for the thread.
				if (osThread->Join (10) == kWaitStatusSuccess)
					break;
			}
		}

		// Remove the thread except it has already removed itself (will happen as
		// part of Join).
		if (s_AttachedThreads->back () == thread)
			s_AttachedThreads->pop_back ();
	}

	// Put finalizer and current thread back in list.
	assert (gcFinalizerThread != NULL && "GC finalizer thread was not found in list of attached threads!");
	if (gcFinalizerThread)
		s_AttachedThreads->push_back (gcFinalizerThread);
	if (currentThread)
		s_AttachedThreads->push_back (currentThread);
#endif
}

void Thread::Detach (Il2CppThread *thread)
{
	assert(thread != NULL && "Cannot detach a NULL thread");

	Uninitialize (thread);
	il2cpp::vm::StackTrace::CleanupStackTracesForCurrentThread();
}

Il2CppThread* Thread::Main ()
{
	return s_MainThread;
}

void Thread::SetMain (Il2CppThread* thread)
{
	s_MainThread = thread;
}

void Thread::SetState (Il2CppThread *thread, ThreadState value)
{
	il2cpp::os::FastAutoLock lock (thread->synch_cs);
	thread->state |= value;
}

ThreadState Thread::GetState (Il2CppThread *thread)
{
	il2cpp::os::FastAutoLock lock (thread->synch_cs);
	return (ThreadState)thread->state;
}

void Thread::ClrState (Il2CppThread* thread, ThreadState state)
{
	il2cpp::os::FastAutoLock lock (thread->synch_cs);
	thread->state &= ~state;
}

const int32_t kMaxThreadStaticSlots = 2048;

void Thread::AdjustStaticData ()
{
	AUTO_LOCK_THREADS ();
	size_t index = 0;
	Il2CppThread* thread = Current ();
	if (!thread->static_data)
		thread->static_data = (void**)IL2CPP_CALLOC (kMaxThreadStaticSlots, sizeof (void*));
	for (std::vector<int32_t>::const_iterator iter = s_ThreadStaticSizes.begin (); iter != s_ThreadStaticSizes.end (); ++iter)
	{
		if (!thread->static_data [index])
			thread->static_data [index] = GarbageCollector::AllocateFixed (*iter, NULL);
		index++;
	}
}

int32_t Thread::AllocThreadStaticData (int32_t size)
{
	AUTO_LOCK_THREADS ();
	int32_t index = (int32_t)s_ThreadStaticSizes.size ();
	assert (index < kMaxThreadStaticSlots);
	s_ThreadStaticSizes.push_back (size);
	for (GCTrackedThreadVector::const_iterator iter = s_AttachedThreads->begin (); iter != s_AttachedThreads->end (); ++iter)
	{
		Il2CppThread* thread = *iter;
		if (!thread->static_data)
			thread->static_data = (void**)IL2CPP_CALLOC (kMaxThreadStaticSlots, sizeof (void*));
		thread->static_data[index] = GarbageCollector::AllocateFixed (size, NULL);
	}

	return index;
}

void Thread::FreeThreadStaticData (Il2CppThread *thread)
{
	AUTO_LOCK_THREADS ();
	size_t index = 0;
	for (std::vector<int32_t>::const_iterator iter = s_ThreadStaticSizes.begin (); iter != s_ThreadStaticSizes.end (); ++iter)
	{
		if (thread->static_data [index])
			GarbageCollector::FreeFixed (thread->static_data [index]);
		index++;
	}
	IL2CPP_FREE (thread->static_data);
	thread->static_data = NULL;
}

void* Thread::GetThreadStaticData (int32_t offset)
{
	// No lock. We allocate static_data once with a fixed size so we can read it
	// safely without a lock here.
	assert(offset >= 0 && static_cast<uint32_t>(offset) < s_ThreadStaticSizes.size ());
	return Current ()->static_data [offset];
}

void Thread::Register (Il2CppThread *thread)
{
	AUTO_LOCK_THREADS ();
	s_AttachedThreads->push_back(thread);
}

void Thread::Unregister (Il2CppThread *thread)
{
	AUTO_LOCK_THREADS ();
	GCTrackedThreadVector::iterator it = std::find(s_AttachedThreads->begin(), s_AttachedThreads->end(), thread);
	assert(it != s_AttachedThreads->end() && "Vm thread not found in list of attached threads.");
	s_AttachedThreads->erase(it);
}

bool Thread::IsVmThread (Il2CppThread *thread)
{
	return !GarbageCollector::IsFinalizerThread (thread);
}

char *Thread::GetName (uint32_t *len)
{
	*len = 0;

	// TODO: not implemented

	return NULL;
}

static void STDCALL CheckCurrentThreadForInterruptCallback(void* context)
{
	Thread::CheckCurrentThreadForInterruptAndThrowIfNecessary();
}

void Thread::RequestInterrupt (Il2CppThread* thread)
{
	il2cpp::os::FastAutoLock lock (thread->synch_cs);

	thread->interruption_requested = true;

	// If thread has already been started, queue an interrupt now.
	il2cpp::os::Thread* osThread = thread->handle;
	if (osThread)
		osThread->QueueUserAPC(CheckCurrentThreadForInterruptCallback, NULL);
}

void Thread::CheckCurrentThreadForInterruptAndThrowIfNecessary()
{
	Il2CppThread* currentThread = il2cpp::vm::Thread::Current ();
	if (!currentThread)
		return;

	il2cpp::os::FastAutoLock lock (currentThread->synch_cs);

	// Don't throw if thread is not currently in waiting state or if there's
	// no pending interrupt.
	if (!currentThread->interruption_requested
		|| !(il2cpp::vm::Thread::GetState (currentThread) & il2cpp::vm::kThreadStateWaitSleepJoin))
		return;

	// Mark the current thread as being unblocked.
	currentThread->interruption_requested = false;
	il2cpp::vm::Thread::ClrState (currentThread, il2cpp::vm::kThreadStateWaitSleepJoin);

	// Throw interrupt exception.
	il2cpp::vm::Exception::Raise (il2cpp::vm::Exception::GetThreadInterruptedException ());
}

static void STDCALL CheckCurrentThreadForAbortCallback(void* context)
{
	Thread::CheckCurrentThreadForAbortAndThrowIfNecessary();
}

void Thread::RequestAbort(Il2CppThread* thread)
{
	il2cpp::os::FastAutoLock lock(thread->synch_cs);

	ThreadState state = il2cpp::vm::Thread::GetState(thread);
	if (state & kThreadStateAbortRequested || state & kThreadStateStopped || state & kThreadStateStopRequested)
		return;

	il2cpp::os::Thread* osThread = thread->handle;
	if (osThread)
	{
		// If thread has already been started, queue an abort now.
		Thread::SetState(thread, kThreadStateAbortRequested);
		osThread->QueueUserAPC(CheckCurrentThreadForAbortCallback, NULL);
	}
	else
	{
		// If thread has not started, put it in the aborted state.
		Thread::SetState(thread, kThreadStateAborted);
	}
}

void Thread::CheckCurrentThreadForAbortAndThrowIfNecessary()
{
	Il2CppThread* currentThread = il2cpp::vm::Thread::Current();
	if (!currentThread)
		return;

	il2cpp::os::FastAutoLock lock(currentThread->synch_cs);

	ThreadState state = il2cpp::vm::Thread::GetState(currentThread);
	if (!(state & kThreadStateAbortRequested))
		return;

	// Mark the current thread as being unblocked.
	il2cpp::vm::Thread::ClrState(currentThread, kThreadStateAbortRequested);

	// Throw interrupt exception.
	Il2CppException* abortException = il2cpp::vm::Exception::GetThreadAbortException();
	IL2CPP_OBJECT_SETREF(currentThread, abort_exc, (Il2CppObject*)abortException);
	il2cpp::vm::Exception::Raise(abortException);
}

void Thread::ResetAbort(Il2CppThread* thread)
{
	il2cpp::vm::Thread::ClrState(thread, kThreadStateAbortRequested);
	if (thread->abort_exc == NULL)
		il2cpp::vm::Exception::Raise(il2cpp::vm::Exception::GetThreadStateException("Unable to reset abort because no abort was requested."));
}

void Thread::MemoryBarrier ()
{
	os::Atomic::MemoryBarrier ();
}

} /* namespace vm */
} /* namespace il2cpp */
