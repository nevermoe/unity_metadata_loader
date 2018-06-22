#include "il2cpp-config.h"
#include <stddef.h>
#include "icalls/mscorlib/System.Reflection/Module.h"
#include "vm/Exception.h"
#include "vm/Module.h"
#include "vm/Image.h"
#include "vm/Array.h"
#include "class-internals.h"
#include "vm/Reflection.h"
#include "vm/String.h"

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
    Il2CppArray* Module::InternalGetTypes(Il2CppReflectionModule * self)
    {
        vm::TypeVector types;
        vm::Image::GetTypes(self->image, true, &types);
        Il2CppArray* result = vm::Array::New(il2cpp_defaults.monotype_class, (il2cpp_array_size_t)types.size());
        size_t index = 0;
        for (vm::TypeVector::const_iterator type = types.begin(); type != types.end(); ++type)
        {
            Il2CppReflectionType* reflectionType = vm::Reflection::GetTypeObject((*type)->byval_arg);
            il2cpp_array_set(result, Il2CppReflectionType*, index, reflectionType);
            index++;
        }

        return result;
    }

    Il2CppString* Module::GetGuidInternal(mscorlib_System_Reflection_Module * thisPtr)
    {
        return il2cpp::vm::String::New("00000000-0000-0000-0000-000000000000");
    }

    Il2CppIntPtr Module::GetHINSTANCE(mscorlib_System_Reflection_Module * thisPtr)
    {
        NOT_IMPLEMENTED_ICALL(Module::GetHINSTANCE);
        return Il2CppIntPtr();
    }

    int32_t Module::get_MetadataToken(Il2CppReflectionModule* self)
    {
        return vm::Module::GetToken(self->image);
    }

    int32_t Module::GetMDStreamVersion(Il2CppIntPtr module_handle)
    {
        NOT_SUPPORTED_IL2CPP(Module::GetMDStreamVersion, "This icall is not supported by il2cpp.");

        return 0;
    }

    Il2CppReflectionType* Module::GetGlobalType(Il2CppReflectionModule* self)
    {
        NOT_SUPPORTED_IL2CPP(Module::GetGlobalType, "This icall is not supported by il2cpp.");

        return 0;
    }

    Il2CppIntPtr Module::ResolveTypeToken(Il2CppIntPtr module, int32_t token, Il2CppArray* type_args, Il2CppArray* method_args, ResolveTokenError* error)
    {
        NOT_SUPPORTED_IL2CPP(Module::ResolveTypeToken, "This icall is not supported by il2cpp.");

        return Il2CppIntPtr();
    }

    Il2CppIntPtr Module::ResolveMethodToken(Il2CppIntPtr module, int32_t token, Il2CppArray* type_args, Il2CppArray* method_args, ResolveTokenError* error)
    {
        NOT_SUPPORTED_IL2CPP(Module::ResolveMethodToken, "This icall is not supported by il2cpp.");

        return Il2CppIntPtr();
    }

    Il2CppIntPtr Module::ResolveFieldToken(Il2CppIntPtr module, int32_t token, Il2CppArray* type_args, Il2CppArray* method_args, ResolveTokenError* error)
    {
        NOT_SUPPORTED_IL2CPP(Module::ResolveFieldToken, "This icall is not supported by il2cpp.");

        return Il2CppIntPtr();
    }

    Il2CppString* Module::ResolveStringToken(Il2CppIntPtr module, int32_t token, ResolveTokenError* error)
    {
        NOT_SUPPORTED_IL2CPP(Module::ResolveStringToken, "This icall is not supported by il2cpp.");

        return 0;
    }

    void* /* System.Reflection.MemberInfo */ Module::ResolveMemberToken(Il2CppIntPtr module, int32_t token, Il2CppArray* type_args, Il2CppArray* method_args, ResolveTokenError* error)
    {
        NOT_SUPPORTED_IL2CPP(Module::ResolveMemberToken, "This icall is not supported by il2cpp.");

        return 0;
    }

    Il2CppArray* Module::ResolveSignature(Il2CppIntPtr module, int32_t metadataToken, ResolveTokenError* error)
    {
        NOT_SUPPORTED_IL2CPP(Module::ResolveSignature, "This icall is not supported by il2cpp.");

        return 0;
    }

    void Module::GetPEKind(Il2CppIntPtr module, PortableExecutableKinds* peKind, ImageFileMachine* machine)
    {
        NOT_SUPPORTED_IL2CPP(Module::GetPEKind, "This icall is not supported by il2cpp.");
    }
} /* namespace Reflection */
} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
