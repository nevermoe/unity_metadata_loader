#include "il2cpp-config.h"
#include <memory>
#include "icalls/mscorlib/System/MonoCustomAttrs.h"
#include "class-internals.h"
#include "object-internals.h"
#include "vm/Array.h"
#include "vm/Class.h"
#include "vm/Object.h"
#include "vm/Reflection.h"
#include "vm/Runtime.h"
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
    Il2CppArray * MonoCustomAttrs::GetCustomAttributesInternal(Il2CppObject* obj, Il2CppReflectionType* type, bool pseudoAttrs)
    {
        IL2CPP_ASSERT(pseudoAttrs == false && "System_MonoCustomAttrs_GetCustomAttributesInternal_icall with pseudoAttrs == true has not been implemented yet");

        CustomAttributesCache *cinfo = il2cpp::vm::Reflection::GetCustomAttrsInfo(obj);

        if (!cinfo)
        {
            return il2cpp::vm::Array::New(il2cpp_defaults.object_class, 0);
        }

        if (!type)
        {
            Il2CppArray *result = il2cpp::vm::Array::New(il2cpp_defaults.object_class, cinfo->count);
            memcpy(il2cpp_array_addr(result, Il2CppObject*, 0), cinfo->attributes, sizeof(Il2CppObject*) * cinfo->count);

            return result;
        }

        Il2CppClass* attributeClass = Class::FromIl2CppType(type->type);
        int count = 0;
        for (int i = 0; i < cinfo->count; i++)
        {
            Il2CppObject* attr = cinfo->attributes[i];
            if (Class::IsAssignableFrom(attributeClass, attr->klass))
                count++;
        }

        Il2CppArray *result = il2cpp::vm::Array::New(il2cpp_defaults.object_class, count);

        int index = 0;
        for (int i = 0; i < cinfo->count; i++)
        {
            Il2CppObject* attr = cinfo->attributes[i];
            if (!Class::IsAssignableFrom(attributeClass, attr->klass))
                continue;

            il2cpp_array_setref(result, index, cinfo->attributes[i]);
            index++;
        }

        return result;
    }

    bool MonoCustomAttrs::IsDefinedInternal(Il2CppObject *obj, Il2CppReflectionType *attr_type)
    {
        return il2cpp::vm::Reflection::HasAttribute(obj, Class::FromIl2CppType(attr_type->type));
    }

#if !NET_4_0
    static Il2CppObject* CreateCustomAttributeData(Il2CppObject* attribute)
    {
        static const MethodInfo* customAttributeDataConstructor;
        void *params[3];

        if (!customAttributeDataConstructor)
            customAttributeDataConstructor = vm::Class::GetMethodFromName(il2cpp_defaults.customattribute_data_class, ".ctor", 3);

        const MethodInfo* attributeConstructor = vm::Class::GetMethodFromName(attribute->klass, ".ctor", 0);

        if (attributeConstructor == NULL)
            NOT_IMPLEMENTED_ICALL(MonoCustomAttrs::GetCustomAttributesDataInternal);

        Il2CppObject* customAttributeData = vm::Object::New(il2cpp_defaults.customattribute_data_class);
        params[0] = vm::Reflection::GetMethodObject(attributeConstructor, NULL);
        params[1] = params[2] = NULL;
        vm::Runtime::Invoke(customAttributeDataConstructor, customAttributeData, params, NULL);
        return customAttributeData;
    }

#else
    static Il2CppObject* CreateCustomAttributeData(Il2CppObject* attribute)
    {
        static const MethodInfo* customAttributeDataConstructor;
        void *params[4];

        if (!customAttributeDataConstructor)
            customAttributeDataConstructor = vm::Class::GetMethodFromName(il2cpp_defaults.customattribute_data_class, ".ctor", 4);

        const MethodInfo* attributeConstructor = vm::Class::GetMethodFromName(attribute->klass, ".ctor", 0);

        if (attributeConstructor == NULL)
            NOT_IMPLEMENTED_ICALL(MonoCustomAttrs::GetCustomAttributesDataInternal);

        Il2CppObject* customAttributeData = vm::Object::New(il2cpp_defaults.customattribute_data_class);
        int argCount = 0;
        params[0] = vm::Reflection::GetMethodObject(attributeConstructor, NULL);
        params[1] = vm::Reflection::GetAssemblyObject(MetadataCache::GetAssemblyFromIndex(attribute->klass->image->assemblyIndex));
        params[2] = &Il2CppIntPtr::Zero;
        params[3] = &argCount;
        vm::Runtime::Invoke(customAttributeDataConstructor, customAttributeData, params, NULL);
        return customAttributeData;
    }

#endif

    Il2CppArray* MonoCustomAttrs::GetCustomAttributesDataInternal(Il2CppObject* obj)
    {
        CustomAttributesCache *cinfo = il2cpp::vm::Reflection::GetCustomAttrsInfo(obj);

        if (!cinfo)
            return il2cpp::vm::Array::New(il2cpp_defaults.customattribute_data_class, 0);

        Il2CppArray* result = il2cpp::vm::Array::New(il2cpp_defaults.customattribute_data_class, cinfo->count);
        for (int i = 0; i < cinfo->count; ++i)
        {
            Il2CppObject* attribute = CreateCustomAttributeData(cinfo->attributes[i]);
            il2cpp_array_setref(result, i, attribute);
        }

        return result;
    }
} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
