#include "il2cpp-config.h"
#include "object-internals.h"
#include "mono-structs.h"

#include "MonoType.h"
#include "RuntimeTypeHandle.h"
#include "Type.h"

#include "vm/Class.h"
#include "vm/Image.h"
#include "vm/Reflection.h"
#include "vm/MetadataCache.h"

#if NET_4_0

namespace il2cpp
{
namespace icalls
{
namespace mscorlib
{
namespace System
{
    bool RuntimeTypeHandle::HasInstantiation(Il2CppReflectionRuntimeType* type)
    {
        return Type::get_IsGenericType(&type->type);
    }

    bool RuntimeTypeHandle::IsArray(Il2CppReflectionRuntimeType* type)
    {
        return Type::IsArrayImpl(&type->type);
    }

    bool RuntimeTypeHandle::IsByRef(Il2CppReflectionRuntimeType* type)
    {
        return MonoType::IsByRefImpl(&type->type);
    }

    bool RuntimeTypeHandle::IsComObject(Il2CppReflectionRuntimeType* type)
    {
        return false; // il2cpp does not support COM objects, so this is always false
    }

    bool RuntimeTypeHandle::IsGenericTypeDefinition(Il2CppReflectionRuntimeType* type)
    {
        return Type::get_IsGenericTypeDefinition(&type->type);
    }

    bool RuntimeTypeHandle::IsGenericVariable(Il2CppReflectionRuntimeType* type)
    {
        return MonoType::get_IsGenericParameter(&type->type);
    }

    bool RuntimeTypeHandle::IsInstanceOfType(Il2CppReflectionRuntimeType* type, Il2CppObject* o)
    {
        return Type::IsInstanceOfType(&type->type, o);
    }

    bool RuntimeTypeHandle::IsPointer(Il2CppReflectionRuntimeType* type)
    {
        return MonoType::IsPointerImpl(&type->type);
    }

    bool RuntimeTypeHandle::IsPrimitive(Il2CppReflectionRuntimeType* type)
    {
        return MonoType::IsPrimitiveImpl(&type->type);
    }

    bool RuntimeTypeHandle::type_is_assignable_from(Il2CppReflectionType* a, Il2CppReflectionType* b)
    {
        return Type::type_is_assignable_from(a, b);
    }

    int32_t RuntimeTypeHandle::GetArrayRank(Il2CppReflectionRuntimeType* type)
    {
        return MonoType::GetArrayRank(&type->type);
    }

    int32_t RuntimeTypeHandle::GetMetadataToken(Il2CppReflectionRuntimeType* type)
    {
        return vm::Class::FromSystemType(&type->type)->token;
    }

    Il2CppReflectionAssembly* RuntimeTypeHandle::GetAssembly(Il2CppReflectionRuntimeType* type)
    {
        return vm::Reflection::GetAssemblyObject(vm::Image::GetAssembly(vm::Class::GetImage(vm::Class::FromIl2CppType(type->type.type))));
    }

    Il2CppReflectionModule* RuntimeTypeHandle::GetModule(Il2CppReflectionRuntimeType* type)
    {
        return vm::Reflection::GetModuleObject(vm::Class::GetImage(vm::Class::FromIl2CppType(type->type.type)));
    }

    int32_t RuntimeTypeHandle::GetAttributes(Il2CppReflectionRuntimeType* type)
    {
        return MonoType::get_attributes(&type->type);
    }

    Il2CppReflectionRuntimeType* RuntimeTypeHandle::GetBaseType(Il2CppReflectionRuntimeType* type)
    {
        return reinterpret_cast<Il2CppReflectionRuntimeType*>(MonoType::get_BaseType(&type->type));
    }

    Il2CppReflectionRuntimeType* RuntimeTypeHandle::GetElementType(Il2CppReflectionRuntimeType* type)
    {
        return reinterpret_cast<Il2CppReflectionRuntimeType*>(MonoType::GetElementType(&type->type));
    }

    Il2CppReflectionType* RuntimeTypeHandle::GetGenericTypeDefinition_impl(Il2CppReflectionRuntimeType* type)
    {
        return Type::GetGenericTypeDefinition_impl(&type->type);
    }

    Il2CppIntPtr RuntimeTypeHandle::GetGenericParameterInfo(Il2CppReflectionRuntimeType* type)
    {
        Il2CppIntPtr retVal;
        retVal.m_value = NULL;

        const Il2CppType *thisType = type->type.type;
        if ((thisType->type == IL2CPP_TYPE_VAR) || (thisType->type == IL2CPP_TYPE_MVAR))
        {
            const Il2CppGenericParameter *param = il2cpp::vm::MetadataCache::GetGenericParameterFromIndex(thisType->data.genericParameterIndex);
            if (param)
            {
                MonoGenericParameterInfo *monoParam = (MonoGenericParameterInfo*)il2cpp::vm::Reflection::GetMonoGenericParameterInfo(param);
                if (monoParam)
                {
                    retVal.m_value = monoParam;
                }
                else
                {
                    monoParam = (MonoGenericParameterInfo*)IL2CPP_MALLOC(sizeof(MonoGenericParameterInfo));
                    monoParam->flags = param->flags;
                    monoParam->token = param->num;
                    monoParam->name = il2cpp::vm::MetadataCache::GetStringFromIndex(param->nameIndex);
                    const Il2CppGenericContainer *container = il2cpp::vm::MetadataCache::GetGenericContainerFromIndex(param->ownerIndex);
                    monoParam->pklass = NULL;
                    if (container)
                        monoParam->pklass = il2cpp::vm::MetadataCache::GetTypeInfoFromTypeIndex(container->ownerIndex);

                    monoParam->constraints = (Il2CppClass**)IL2CPP_MALLOC(sizeof(Il2CppClass*) * (param->constraintsCount + 1));
                    for (int i = 0; i < param->constraintsCount; ++i)
                    {
                        const Il2CppType *constraintType = il2cpp::vm::MetadataCache::GetGenericParameterConstraintFromIndex(param->constraintsStart + i);
                        monoParam->constraints[i] = il2cpp::vm::Class::FromIl2CppType(constraintType);
                    }

                    monoParam->constraints[param->constraintsCount] = NULL;

                    il2cpp::vm::Reflection::SetMonoGenericParameterInfo(param, monoParam);
                    retVal.m_value = monoParam;
                }
            }
        }

        return retVal;
    }
} // namespace System
} // namespace mscorlib
} // namespace icalls
} // namespace il2cpp

#endif
