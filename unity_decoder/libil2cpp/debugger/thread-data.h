#pragma once

#include <map>
#include <vector>

#include "common.h"

#include "protocol/globals.h"

struct Il2CppThread;
struct Il2CppStackFrameInfo;

namespace il2cpp
{

namespace os
{

class Mutex;
class Event;

} /* namespace os */

namespace debugger
{

struct MethodInvokeData
{
	MethodInvokeData() :
		method_to_invoke(NULL),
		return_value(NULL)
	{}

	const MethodInfo* method_to_invoke;
	Il2CppObject* return_value;
};

static int32_t frame_id = 0;

class ThreadData
{

public:

	explicit ThreadData(Il2CppThread *thread);
	~ThreadData();

	inline Il2CppThread *thread()
	{
		return _thread;
	}

	inline std::vector<const Il2CppStackFrameInfo*> &frames_cache()
	{
		return _frames_cache;
	}

	inline const std::vector<const Il2CppStackFrameInfo*> &frames_cache() const
	{
		return _frames_cache;
	}

	void UpdateFramesCacheIfNeeded();
	void InvalidateFramesCache();
	
	bool IsFramesCacheValid();
	const Il2CppStackFrameInfo *FrameById(int32_t id);

	void SetMethodToInvoke(const MethodInfo* method);
	const MethodInfo* GetMethodToInvoke () const;
	void ClearMethodToInvoke();
	void SetReturnValueOfMethodToInvoke(Il2CppObject* return_value);
	Il2CppObject* GetReturnValueOfMethodToInvoke() const;
	void WaitForMethodToBeInvoked();
	void SignalMethodInvokeComplete();

private:

	static void UpdateStackFrame(const Il2CppStackFrameInfo *info, void *user_data);

	Il2CppThread *_thread;
	const std::auto_ptr<os::Mutex>_write_sync;
	int32_t _is_frames_cache_valid;
	std::vector<const Il2CppStackFrameInfo*> _frames_cache;

	const std::auto_ptr<os::Mutex> _method_to_invoke_sync;
	const std::auto_ptr<os::Event> _method_to_invoke_complete;
	MethodInvokeData _method_to_invoke_data;

	DISALLOW_COPY(ThreadData);
};

class ThreadDataMap
{

public:

	ThreadDataMap();
	~ThreadDataMap();

	void NotifyThreadAttach(Il2CppThread *thread);
	void NotifyThreadDetach(Il2CppThread *thread);
	
	bool HasThreadDataForThread(const Il2CppThread *thread) const;
	bool HasThreadDataForCurrentThread() const;

	ThreadData *ThreadDataFor(const Il2CppThread *thread);

private:
	
	os::Mutex *_thread_to_data_sync;
	std::map<const Il2CppThread*, ThreadData*> _thread_to_data;

	DISALLOW_COPY(ThreadDataMap);
};

} /* namespace debugger */
} /* namespace il2cpp */
