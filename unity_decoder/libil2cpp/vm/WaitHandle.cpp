#include "il2cpp-config.h"
#include "class-internals.h"
#include "object-internals.h"
#include "vm/WaitHandle.h"

#include "vm/Class.h"
#include "vm/Field.h"
#include "vm/Object.h"
#include "vm/Runtime.h"

namespace il2cpp
{
namespace vm
{
    Il2CppWaitHandle* WaitHandle::NewManualResetEvent(bool initialState)
    {
        static const MethodInfo* constructor = NULL;
        if (!constructor)
            constructor = Class::GetMethodFromName(il2cpp_defaults.manualresetevent_class, ".ctor", 1);

        Il2CppObject* instance = Object::New(il2cpp_defaults.manualresetevent_class);
        void* args[1] = { &initialState };
        // NOTE: passing NULL here as Mono does, as long as the WaitHandle ctor will never throw an exception.
        Runtime::Invoke(constructor, instance, args, NULL);

        return reinterpret_cast<Il2CppWaitHandle*>(instance);
    }

    os::Handle* WaitHandle::GetPlatformHandle(Il2CppWaitHandle* waitHandle)
    {
        static FieldInfo *s_osHandle;
        static FieldInfo *s_safeHandle;

        if (!s_osHandle && !s_safeHandle)
        {
#if !NET_4_0
            s_osHandle = vm::Class::GetFieldFromName(il2cpp_defaults.manualresetevent_class, "os_handle");
            s_safeHandle = vm::Class::GetFieldFromName(il2cpp_defaults.manualresetevent_class, "safe_wait_handle");
#else
            s_osHandle = vm::Class::GetFieldFromName(il2cpp_defaults.manualresetevent_class, "Handle");
            s_safeHandle = vm::Class::GetFieldFromName(il2cpp_defaults.manualresetevent_class, "safeWaitHandle");
#endif
        }

        Il2CppIntPtr retval;
        if (s_osHandle)
        {
            vm::Field::GetValue((Il2CppObject*)waitHandle, s_osHandle, &retval);
        }
        else
        {
            Il2CppSafeHandle *sh;
            vm::Field::GetValue((Il2CppObject*)waitHandle, s_safeHandle, &sh);
            retval.m_value = sh->handle;
        }

        return static_cast<os::Handle*>(retval.m_value);
    }
} /* namespace vm */
} /* namespace il2cpp */
