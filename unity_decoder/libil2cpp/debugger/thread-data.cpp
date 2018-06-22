#if IL2CPP_DEBUGGER_ENABLED

#include "thread-data.h"

#include "../os/Mutex.h"
#include "../os/Atomic.h"
#include "../os/Event.h"
#include "../il2cpp-api.h"

using namespace il2cpp::debugger;

ThreadData::ThreadData(Il2CppThread *thread) :
    _thread(thread),
    _write_sync(new os::Mutex),
    _is_frames_cache_valid(0),
    _method_to_invoke_sync(new os::Mutex),
    _method_to_invoke_complete(new os::Event)
{
}

ThreadData::~ThreadData()
{
    _frames_cache.clear();
    _thread = 0;
}

struct ThreadDataUpdateInfo
{
    ThreadData *data;
    std::vector<const Il2CppStackFrameInfo*> new_frames;
};

bool ThreadData::IsFramesCacheValid()
{
    return os::Atomic::CompareExchange(&_is_frames_cache_valid, _is_frames_cache_valid, 0) != 0;
}

const Il2CppStackFrameInfo *ThreadData::FrameById(int32_t id)
{
    std::vector<const Il2CppStackFrameInfo*>::const_iterator fit = _frames_cache.begin();

    while (fit != _frames_cache.end())
    {
        const Il2CppStackFrameInfo *frame = *fit;
        if (frame->id == id)
            return frame;

        ++fit;
    }

    return 0;
}

void ThreadData::UpdateFramesCacheIfNeeded()
{
    os::AutoLock lock(_write_sync.get());

    if (os::Atomic::CompareExchange(&_is_frames_cache_valid, _is_frames_cache_valid, 0) != 0)
        return;

    ThreadDataUpdateInfo info;
    info.data = this;

    il2cpp_thread_walk_frame_stack(_thread, ThreadData::UpdateStackFrame, &info);

    _frames_cache = info.new_frames;

    os::Atomic::Increment(&_is_frames_cache_valid);
}

void ThreadData::UpdateStackFrame(const Il2CppStackFrameInfo *info, void *user_data)
{
    ThreadDataUpdateInfo *data = (ThreadDataUpdateInfo*)user_data;

    Il2CppStackFrameInfo *w_info = const_cast<Il2CppStackFrameInfo*>(info);

    if (w_info->id != -1)
    {
        // Let's try to recycle the frame id if possible
        std::vector<const Il2CppStackFrameInfo*>::const_iterator it = data->data->frames_cache().begin();
        std::vector<const Il2CppStackFrameInfo*>::const_iterator end = data->data->frames_cache().end();
        while (it != end)
        {
            const Il2CppStackFrameInfo *frame = *it;
            if ((uintptr_t)frame == (uintptr_t)info && (uintptr_t)frame->locals == (uintptr_t)info->locals)
            {
                // In this case, the frame is considered to be unchanged and we can keep the old ID.
                break;
            }
            ++it;
        }

        if (it == end)
            w_info->id = os::Atomic::Increment(&frame_id);
    }
    else
        w_info->id = os::Atomic::Increment(&frame_id);

    data->new_frames.push_back(info);
}

void ThreadData::InvalidateFramesCache()
{
    os::AutoLock lock(_write_sync.get());

    if (os::Atomic::CompareExchange(&_is_frames_cache_valid, _is_frames_cache_valid, 0) == 0)
        return;

    // Invalidate the frames cache
    os::Atomic::Decrement(&_is_frames_cache_valid);
}

void ThreadData::SetMethodToInvoke(const MethodInfo* method)
{
    os::AutoLock lock(_method_to_invoke_sync.get());

    IL2CPP_ASSERT(_method_to_invoke_data.method_to_invoke == NULL && "ThreadData::SetMethodToInvoke called with a method to invoke, when one is already waiting to be invoked");

    _method_to_invoke_data.method_to_invoke = method;
}

const MethodInfo* ThreadData::GetMethodToInvoke() const
{
    os::AutoLock lock(_method_to_invoke_sync.get());

    return _method_to_invoke_data.method_to_invoke;
}

void ThreadData::ClearMethodToInvoke()
{
    os::AutoLock lock(_method_to_invoke_sync.get());

    _method_to_invoke_data.method_to_invoke = NULL;
}

void ThreadData::SetReturnValueOfMethodToInvoke(Il2CppObject* return_value)
{
    os::AutoLock lock(_method_to_invoke_sync.get());

    _method_to_invoke_data.return_value = return_value;
}

Il2CppObject* ThreadData::GetReturnValueOfMethodToInvoke() const
{
    os::AutoLock lock(_method_to_invoke_sync.get());

    return _method_to_invoke_data.return_value;
}

void ThreadData::WaitForMethodToBeInvoked()
{
    _method_to_invoke_complete->Wait();
}

void ThreadData::SignalMethodInvokeComplete()
{
    _method_to_invoke_complete->Set();
}

ThreadDataMap::ThreadDataMap() :
    _thread_to_data_sync(0)
{
    _thread_to_data_sync = new os::Mutex();
}

ThreadDataMap::~ThreadDataMap()
{
    std::map<const Il2CppThread*, ThreadData*>::iterator it = _thread_to_data.begin();

    while (it != _thread_to_data.end())
    {
        ThreadData* data = it->second;
        delete data;

        ++it;
    }

    _thread_to_data.clear();

    delete _thread_to_data_sync;
    _thread_to_data_sync = 0;
}

void ThreadDataMap::NotifyThreadAttach(Il2CppThread *thread)
{
    os::AutoLock lock(_thread_to_data_sync);

    IL2CPP_ASSERT(_thread_to_data.find(thread) == _thread_to_data.end() && "ThreadDataMap::NotifyThreadAttach is being called twice on the same thread");

    _thread_to_data[thread] = new ThreadData(thread);
}

void ThreadDataMap::NotifyThreadDetach(Il2CppThread *thread)
{
    os::AutoLock lock(_thread_to_data_sync);

    std::map<const Il2CppThread*, ThreadData*>::const_iterator it = _thread_to_data.find(thread);

    IL2CPP_ASSERT(it != _thread_to_data.end() && "ThreadDataMap::NotifyThreadDetach is being called on a thread that was not attached");

    delete it->second;

    _thread_to_data.erase(it);
}

bool ThreadDataMap::HasThreadDataForThread(const Il2CppThread *thread) const
{
    os::AutoLock lock(_thread_to_data_sync);

    return _thread_to_data.find(thread) != _thread_to_data.end();
}

bool ThreadDataMap::HasThreadDataForCurrentThread() const
{
    return HasThreadDataForThread(il2cpp_thread_current());
}

ThreadData *ThreadDataMap::ThreadDataFor(const Il2CppThread *thread)
{
    os::AutoLock lock(_thread_to_data_sync);

    std::map<const Il2CppThread*, ThreadData*>::const_iterator it = _thread_to_data.find(thread);

    if (it == _thread_to_data.end())
        return NULL;

    return it->second;
}

#endif
