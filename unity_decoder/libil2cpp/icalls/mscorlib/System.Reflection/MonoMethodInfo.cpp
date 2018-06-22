#include "il2cpp-config.h"
#include <stddef.h>
#include "icalls/mscorlib/System.Reflection/MonoMethodInfo.h"
#include "class-internals.h"
#include "vm/Class.h"
#include "vm/Reflection.h"
#include "vm/Exception.h"

using namespace il2cpp::vm;

namespace il2cpp
{
namespace icalls
{
namespace mscorlib
{
namespace System
{
namespace Reflection
{
    void MonoMethodInfo::get_method_info(Il2CppIntPtr methodPtr, Il2CppMethodInfo *info)
    {
        NOT_IMPLEMENTED_ICALL_NO_ASSERT(MonoMethodInfo::get_method_info, "Audit and look over commented code. Work in progress.");

        MethodInfo* method = (MethodInfo*)methodPtr.m_value;
        //MonoMethodSignature* sig;

        //sig = mono_method_signature (method);
        //if (!sig) {
        //  IL2CPP_ASSERT(mono_loader_get_last_error ());
        //  mono_raise_exception (mono_loader_error_prepare_exception (mono_loader_get_last_error ()));
        //}

        IL2CPP_STRUCT_SETREF(info, parent, il2cpp::vm::Reflection::GetTypeObject(method->declaring_type->byval_arg));
        if (method->return_type)
            IL2CPP_STRUCT_SETREF(info, ret, il2cpp::vm::Reflection::GetTypeObject(method->return_type));
        info->attrs = method->flags;
        info->implattrs = method->iflags;
        //if (sig->call_convention == MONO_CALL_DEFAULT)
        //  info->callconv = sig->sentinelpos >= 0 ? 2 : 1;
        //else {
        //  if (sig->call_convention == MONO_CALL_VARARG || sig->sentinelpos >= 0)
        //      info->callconv = 2;
        //  else
        //      info->callconv = 1;
        //}
        //info->callconv |= (sig->hasthis << 5) | (sig->explicit_this << 6);
    }

    Il2CppArray * MonoMethodInfo::get_parameter_info(Il2CppIntPtr methodPtr, Il2CppReflectionMethod *member)
    {
        MethodInfo* method = (MethodInfo*)methodPtr.m_value;
        return il2cpp::vm::Reflection::GetParamObjects(method, member->reftype ? Class::FromIl2CppType(member->reftype->type) : NULL);
    }

    void* /* System.Reflection.Emit.UnmanagedMarshal */ MonoMethodInfo::get_retval_marshal(Il2CppIntPtr handle)
    {
        NOT_IMPLEMENTED_ICALL_NO_ASSERT(MonoMethodInfo::get_retval_marshal, "This icall is not supported by il2cpp.");

        return NULL;
    }

#if NET_4_0
    int32_t MonoMethodInfo::get_method_attributes(Il2CppIntPtr methodPtr)
    {
        MethodInfo* method = (MethodInfo*)methodPtr.m_value;
        return method->flags;
    }

#endif
} /* namespace Reflection */
} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
