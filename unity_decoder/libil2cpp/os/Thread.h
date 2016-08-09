#pragma once

#include "os/ErrorCodes.h"
#include "os/Handle.h"
#include "os/Event.h"
#include "os/WaitStatus.h"
#include "utils/NonCopyable.h"

namespace il2cpp
{
namespace os
{

class ThreadImpl;

enum ThreadPriority
{
	kThreadPriorityNormal,
	kThreadPriorityLow,
	kThreadPriorityHigh
};

enum ApartmentState
{
	kApartmentStateInSTA = 0,
	kApartmentStateInMTA = 1,
	kApartmentStateUnknown = 2,
	kApartmentStateCoInitialized = 4,
};

class Thread : public il2cpp::utils::NonCopyable
{
public:
	Thread();
	~Thread();

	typedef void (*StartFunc) (void* arg);
	// Use STDCALL calling convention on Windows, as it will be called back directly from the OS. This is defined as nothing on other platforms.
	typedef void (STDCALL *APCFunc) (void* context);
	typedef uint64_t ThreadId;
	typedef void (*CleanupFunc) (void* arg);

	/// Initialize/Shutdown thread subsystem. Must be called on main thread.
	static void Init ();
	static void Shutdown ();

	ErrorCode Run (StartFunc func, void* arg);
	ThreadId Id ();

	/// Set thread name for debugging purposes. Won't do anything if not supported
	/// by platform.
	void SetName (const std::string& name);

	void SetPriority (ThreadPriority priority);

	void SetStackSize (size_t stackSize);

	void SetCleanupFunction (CleanupFunc cleanupFunc, void* arg)
	{
		m_CleanupFunc = cleanupFunc;
		m_CleanupFuncArg = arg;
	}

	/// Interruptible, infinite wait join.
	WaitStatus Join ();

	/// Interruptible, timed wait join.
	WaitStatus Join (uint32_t ms);

	/// Execute the given function on the thread the next time the thread executes
	/// an interruptible blocking operation.
	/// NOTE: The APC is allowed to raise exceptions!
	void QueueUserAPC (APCFunc func, void* context);

	// Explicit versions modify state without actually changing COM state.
	// Used to set thread state before it's started.
	ApartmentState GetApartment();
	ApartmentState GetExplicitApartment();
	ApartmentState SetApartment(ApartmentState state);
	void SetExplicitApartment(ApartmentState state);

	/// Interruptible, timed sleep.
	static void Sleep (uint32_t ms, bool interruptible = false);

	static ThreadId CurrentThreadId ();
	static Thread* GetCurrentThread ();
	static Thread* GetOrCreateCurrentThread ();
	static void DetachCurrentThread ();

#if IL2CPP_HAS_NATIVE_THREAD_CLEANUP
	typedef void (*ThreadCleanupFunc) (void* arg);
	static void SetNativeThreadCleanup(ThreadCleanupFunc cleanupFunction);
	static void RegisterCurrentThreadForCleanup (void* arg);
	static void UnregisterCurrentThreadForCleanup ();
#endif

	static const uint64_t kInvalidThreadId = 0;

private:

	enum ThreadState
	{
		kThreadCreated,
		kThreadRunning,
		kThreadWaiting,
		kThreadExited
	};

	ThreadState m_State;

	friend class ThreadImpl; // m_Thread

	ThreadImpl* m_Thread;

	/// Event that the thread signals when it finishes execution. Used for joins.
	/// Supports interruption.
	Event m_ThreadExitedEvent;

	CleanupFunc m_CleanupFunc;
	void* m_CleanupFuncArg;

	Thread (ThreadImpl* thread)
		: m_Thread (thread) {}

	static void RunWrapper (void* arg);
};

}
}
