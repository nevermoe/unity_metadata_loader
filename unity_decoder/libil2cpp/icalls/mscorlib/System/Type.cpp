#include "il2cpp-config.h"
#include "icalls/mscorlib/System/Type.h"
#include "icalls/mscorlib/System/MonoType.h"
#include "class-internals.h"
#include "object-internals.h"
#include "il2cpp-api.h"
#include <sstream>
#include "tabledefs.h"
#include "metadata/GenericMetadata.h"
#include "metadata/Il2CppTypeVector.h"
#include "vm/Array.h"
#include "vm/Assembly.h"
#include "vm/Class.h"
#include "vm/GenericClass.h"
#include "vm/MetadataCache.h"
#include "vm/Object.h"
#include "vm/Runtime.h"
#include "vm/String.h"
#include "vm/Type.h"
#include "vm/Thread.h"
#include "vm/Exception.h"
#include "vm/Reflection.h"
#include "utils/StringUtils.h"

#include <vector>

using namespace il2cpp::vm;
using il2cpp::metadata::GenericMetadata;
using il2cpp::metadata::Il2CppTypeVector;
using il2cpp::utils::StringUtils;

namespace il2cpp
{
namespace icalls
{
namespace mscorlib
{
namespace System
{
    bool Type::EqualsInternal(Il2CppReflectionType * left, Il2CppReflectionType * right)
    {
        return (left->type == right->type);
    }

    bool Type::get_IsGenericType(Il2CppReflectionType* type)
    {
        Il2CppClass *klass;

        NOT_IMPLEMENTED_ICALL_NO_ASSERT(MonoType::get_IsGenericTypeDefinition, "Check for custom Type implementations");
        //if (!IS_MONOTYPE (type))
        //  return FALSE;

        if (type->type->byref)
            return false;

        klass = Class::FromIl2CppType(type->type);
        return klass->generic_class != NULL || Class::IsGeneric(klass);
    }

    bool Type::get_IsGenericTypeDefinition(Il2CppReflectionType * type)
    {
        Il2CppClass *klass;

        NOT_IMPLEMENTED_ICALL_NO_ASSERT(MonoType::get_IsGenericTypeDefinition, "Check for custom Type implementations");
        //if (!IS_MONOTYPE (type))
        //  return FALSE;

        if (type->type->byref)
            return false;

        klass = Class::FromIl2CppType(type->type);

        return Class::IsGeneric(klass);
    }

    int32_t Type::GetGenericParameterPosition(Il2CppReflectionType* type)
    {
        if (MonoType::get_IsGenericParameter(type))
            return vm::Type::GetGenericParameter(type->type)->num;
        return -1;
    }

    Il2CppReflectionType * Type::GetGenericTypeDefinition_impl(Il2CppReflectionType* type)
    {
        Il2CppClass *klass;

        if (type->type->byref)
            return NULL;

        klass = Class::FromIl2CppType(type->type);
        if (Class::IsGeneric(klass))
            return type;

        if (klass->generic_class)
        {
            Il2CppClass *generic_class = GenericClass::GetTypeDefinition(klass->generic_class);
            return Reflection::GetTypeObject(generic_class->byval_arg);
        }

        return NULL;
    }

/* System.TypeCode */
    typedef enum
    {
        TYPECODE_EMPTY,
        TYPECODE_OBJECT,
        TYPECODE_DBNULL,
        TYPECODE_BOOLEAN,
        TYPECODE_CHAR,
        TYPECODE_SBYTE,
        TYPECODE_BYTE,
        TYPECODE_INT16,
        TYPECODE_UINT16,
        TYPECODE_INT32,
        TYPECODE_UINT32,
        TYPECODE_INT64,
        TYPECODE_UINT64,
        TYPECODE_SINGLE,
        TYPECODE_DOUBLE,
        TYPECODE_DECIMAL,
        TYPECODE_DATETIME,
        TYPECODE_STRING = 18
    } TypeCode;

    int Type::GetTypeCodeInternal(Il2CppReflectionType* type)
    {
        int t = type->type->type;

        if (type->type->byref)
            return TYPECODE_OBJECT;

    handle_enum:
        switch (t)
        {
            case IL2CPP_TYPE_VOID:
                return TYPECODE_OBJECT;
            case IL2CPP_TYPE_BOOLEAN:
                return TYPECODE_BOOLEAN;
            case IL2CPP_TYPE_U1:
                return TYPECODE_BYTE;
            case IL2CPP_TYPE_I1:
                return TYPECODE_SBYTE;
            case IL2CPP_TYPE_U2:
                return TYPECODE_UINT16;
            case IL2CPP_TYPE_I2:
                return TYPECODE_INT16;
            case IL2CPP_TYPE_CHAR:
                return TYPECODE_CHAR;
            case IL2CPP_TYPE_PTR:
            case IL2CPP_TYPE_U:
            case IL2CPP_TYPE_I:
                return TYPECODE_OBJECT;
            case IL2CPP_TYPE_U4:
                return TYPECODE_UINT32;
            case IL2CPP_TYPE_I4:
                return TYPECODE_INT32;
            case IL2CPP_TYPE_U8:
                return TYPECODE_UINT64;
            case IL2CPP_TYPE_I8:
                return TYPECODE_INT64;
            case IL2CPP_TYPE_R4:
                return TYPECODE_SINGLE;
            case IL2CPP_TYPE_R8:
                return TYPECODE_DOUBLE;
            case IL2CPP_TYPE_VALUETYPE:
            {
                if (vm::Type::IsEnum(type->type))
                {
                    t = Class::GetEnumBaseType(vm::Type::GetClass(type->type))->type;
                    goto handle_enum;
                }
                else
                {
                    if (vm::Type::IsSystemDecimal(type->type))
                        return TYPECODE_DECIMAL;
                    else if (vm::Type::IsSystemDateTime(type->type))
                        return TYPECODE_DATETIME;
                }
                return TYPECODE_OBJECT;
            }
            case IL2CPP_TYPE_STRING:
                return TYPECODE_STRING;
            case IL2CPP_TYPE_SZARRAY:
            case IL2CPP_TYPE_ARRAY:
            case IL2CPP_TYPE_OBJECT:
            case IL2CPP_TYPE_VAR:
            case IL2CPP_TYPE_MVAR:
            case IL2CPP_TYPE_TYPEDBYREF:
                return TYPECODE_OBJECT;
            case IL2CPP_TYPE_CLASS:
            {
                if (vm::Type::IsSystemDBNull(type->type))
                    return TYPECODE_DBNULL;
            }
                return TYPECODE_OBJECT;
            case IL2CPP_TYPE_GENERICINST:
                return TYPECODE_OBJECT;
            default:
                abort();
        }
        return false;
    }

    Il2CppReflectionType * Type::internal_from_handle(Il2CppIntPtr ptr)
    {
        const Il2CppType* type = (const Il2CppType*)ptr.m_value;
        Il2CppClass *klass = Class::FromIl2CppType(type);

        return il2cpp::vm::Reflection::GetTypeObject(klass->byval_arg);
    }

#define CHECK_IF_NULL(v)    \
    if ( (v) == NULL && throwOnError ) \
        Exception::Raise (Exception::GetTypeLoadException ()); \
    if ( (v) == NULL ) \
        return NULL;

    Il2CppReflectionType * Type::internal_from_name(Il2CppString* name, bool throwOnError, bool ignoreCase)
    {
        std::string str = StringUtils::Utf16ToUtf8(utils::StringUtils::GetChars(name));

        il2cpp::vm::TypeNameParseInfo info;
        il2cpp::vm::TypeNameParser parser(str, info, false);

        if (!parser.Parse())
        {
            if (throwOnError)
                Exception::Raise(Exception::GetArgumentException("typeName", "Invalid type name"));
            else
                return NULL;
        }

        const Il2CppType *type = vm::Class::il2cpp_type_from_type_info(info, throwOnError, ignoreCase);

        CHECK_IF_NULL(type);

        return il2cpp::vm::Reflection::GetTypeObject(type);
    }

    bool Type::IsArrayImpl(Il2CppReflectionType *t)
    {
        NOT_IMPLEMENTED_ICALL_NO_ASSERT(Type::IsArrayImpl, "Faulty implementation?");

        Il2CppClass* typeInfo = Class::FromSystemType(t);
        return typeInfo->rank > 0;
    }

    bool Type::IsInstanceOfType(Il2CppReflectionType *type, Il2CppObject * obj)
    {
        Il2CppClass *klass = Class::FromIl2CppType(type->type);
        return il2cpp::vm::Object::IsInst(obj, klass) != NULL;
    }

    void validate_make_array_type_inputs(Il2CppReflectionType* type, int32_t rank)
    {
        // Per MSDN: http://msdn.microsoft.com/en-us/library/w0ykk2sw(v=vs.110).aspx
        if (rank > 32)
        {
            std::stringstream message;
            message << vm::Type::GetName(type->type, IL2CPP_TYPE_NAME_FORMAT_FULL_NAME) << " with rank " << rank << " has too many dimensions.";
            il2cpp_raise_exception(vm::Exception::GetTypeLoadException(message.str().c_str()));
        }

        if (type->type->byref)
        {
            std::stringstream message;
            message << "Could not create array type '" << vm::Type::GetName(type->type, IL2CPP_TYPE_NAME_FORMAT_FULL_NAME) << "'.";
            il2cpp_raise_exception(vm::Exception::GetTypeLoadException(message.str().c_str()));
        }

        const Il2CppClass *klass = Class::FromIl2CppType(type->type);

        if ((strcmp(klass->namespaze, "System") == 0 && strcmp(klass->name, "TypedReference") == 0))
        {
            std::stringstream message;
            message << "Could not create array type '" << klass->namespaze << "." << klass->name << "[]'.";
            il2cpp_raise_exception(vm::Exception::GetTypeLoadException(message.str().c_str()));
        }
    }

    Il2CppReflectionType* Type::make_array_type(Il2CppReflectionType* type, int32_t rank)
    {
        validate_make_array_type_inputs(type, rank);

        Il2CppClass* arrayClass;

        Il2CppClass* klass = il2cpp_class_from_il2cpp_type(type->type);
        if (rank == 0) //single dimentional array
            arrayClass = il2cpp_array_class_get(klass, 1);
        else
            arrayClass = il2cpp_bounded_array_class_get(klass, rank, true);

        return arrayClass != NULL ? Reflection::GetTypeObject(arrayClass->byval_arg) : NULL;
    }

    static std::string FormatExceptionMessageForNonConstructableGenericType(const Il2CppType* type, const Il2CppTypeVector& genericArguments)
    {
        std::string message;
        message += "Failed to construct generic type '";
        message += il2cpp::vm::Type::GetName(type, IL2CPP_TYPE_NAME_FORMAT_FULL_NAME);
        message += "' with generic arguments [";
        for (Il2CppTypeVector::const_iterator iter = genericArguments.begin(); iter != genericArguments.end(); ++iter)
        {
            if (iter != genericArguments.begin())
                message += ", ";
            message += il2cpp::vm::Type::GetName(*iter, IL2CPP_TYPE_NAME_FORMAT_FULL_NAME);
        }
        message += "] at runtime.";

        return message;
    }

    Il2CppReflectionType * Type::MakeGenericType(Il2CppReflectionType* type, Il2CppArray* genericArgumentTypes)
    {
        const Il2CppType* genericTypeDefinitionType = type->type;
        Il2CppClass* genericTypeDefinitionClass = Class::FromIl2CppType(genericTypeDefinitionType);
        IL2CPP_ASSERT(Class::IsGeneric(genericTypeDefinitionClass));

        uint32_t arrayLength = Array::GetLength(genericArgumentTypes);
        Il2CppTypeVector genericArguments;
        genericArguments.reserve(arrayLength);

        for (uint32_t i = 0; i < arrayLength; i++)
        {
            Il2CppReflectionType* genericArgumentType = il2cpp_array_get(genericArgumentTypes, Il2CppReflectionType*, i);
            genericArguments.push_back(genericArgumentType->type);
        }

        const Il2CppGenericInst* inst = MetadataCache::GetGenericInst(genericArguments);
        Il2CppGenericClass* genericClass = GenericMetadata::GetGenericClass(genericTypeDefinitionClass, inst);
        Il2CppClass* genericInstanceTypeClass = GenericClass::GetClass(genericClass);

        if (!genericInstanceTypeClass)
        {
            Exception::Raise(Exception::GetNotSupportedException(FormatExceptionMessageForNonConstructableGenericType(genericTypeDefinitionType, genericArguments).c_str()));
            return NULL;
        }

        return Reflection::GetTypeObject(genericInstanceTypeClass->byval_arg);
    }

    bool Type::type_is_assignable_from(Il2CppReflectionType * type, Il2CppReflectionType * c)
    {
        Il2CppClass *klass;
        Il2CppClass *klassc;

        klass = Class::FromIl2CppType(type->type);
        klassc = Class::FromIl2CppType(c->type);

        if (type->type->byref && !c->type->byref)
            return false;

        return Class::IsAssignableFrom(klass, klassc);
    }

    bool Type::type_is_subtype_of(Il2CppReflectionType *type, Il2CppReflectionType *c, bool check_interfaces)
    {
        Il2CppClass *klass;
        Il2CppClass *klassc;

        IL2CPP_ASSERT(type != NULL);

        if (!c) /* FIXME: dont know what do do here */
            return false;

        klass = Class::FromSystemType(type);
        klassc = Class::FromSystemType(c);

        /*if (type->type->byref)
            return klassc == mono_defaults.object_class;*/

        return Class::IsSubclassOf(klass, klassc, check_interfaces);
    }

    Il2CppReflectionType* Type::make_byref_type(Il2CppReflectionType *type)
    {
        Il2CppClass *klass;

        klass = Class::FromIl2CppType(type->type);

        return il2cpp::vm::Reflection::GetTypeObject(klass->this_arg);
    }

    Il2CppReflectionType * Type::MakePointerType(Il2CppReflectionType* type)
    {
        Il2CppClass* pointerType = Class::GetPtrClass(type->type);

        return Reflection::GetTypeObject(pointerType->byval_arg);
    }

    void Type::GetInterfaceMapData(Il2CppReflectionType* type, Il2CppReflectionType* iface, Il2CppArray** targets, Il2CppArray** methods)
    {
        Il2CppClass* klass = il2cpp_class_from_il2cpp_type(type->type);
        Il2CppClass* iklass = il2cpp_class_from_il2cpp_type(iface->type);

        int32_t numberOfMethods = (int32_t)Class::GetNumMethods(iklass);
        *targets = il2cpp_array_new(il2cpp_defaults.method_info_class, numberOfMethods);
        *methods = il2cpp_array_new(il2cpp_defaults.method_info_class, numberOfMethods);

        if (numberOfMethods == 0)
            return;

        void* unused = NULL;
        Class::Init(klass);
        const VirtualInvokeData* invokeDataStart;

        // So this part is tricky. GetInterfaceInvokeDataFromVTable takes an object pointer in order to support
        // COM peculiarities, like being able to return invoke data for an interface only if native side implements it
        // So here we create a fake object of the class we want to query and pass that to GetInterfaceInvokeDataFromVTable
        // It is safe because the only fields GetInterfaceInvokeDataFromVTable accesses are the klass and identity fields
        if (!klass->is_import_or_windows_runtime)
        {
            Il2CppObject fakeObject;
            fakeObject.klass = klass;
            fakeObject.monitor = NULL;
            invokeDataStart = &Class::GetInterfaceInvokeDataFromVTable(&fakeObject, iklass, 0);
        }
        else
        {
            Il2CppComObject fakeComObject;
            fakeComObject.klass = klass;
            fakeComObject.monitor = NULL;

            // This makes GetInterfaceInvokeDataFromVTable believe that the COM object is dead,
            // thus making it skip asking native side whether a particular interface is supported
            fakeComObject.identity = NULL;

            invokeDataStart = &Class::GetInterfaceInvokeDataFromVTable(&fakeComObject, iklass, 0);
        }

        for (int i = 0; i < numberOfMethods; ++i)
        {
            const MethodInfo *method = il2cpp_class_get_methods(iklass, &unused);
            Il2CppReflectionMethod* member = il2cpp_method_get_object(method, iklass);
            il2cpp_array_setref(*methods, i, member);
            member = il2cpp_method_get_object(invokeDataStart[i].method, klass);
            il2cpp_array_setref(*targets, i, member);
        }
    }

    GenericParameterAttributes Type::GetGenericParameterAttributes(Il2CppReflectionType* type)
    {
        const Il2CppGenericParameter* genericParameter = vm::Type::GetGenericParameter(type->type);
        if (genericParameter->ownerIndex == kGenericContainerIndexInvalid)
            return 0;

        return genericParameter->flags;
    }

    Il2CppArray* Type::GetGenericParameterConstraints_impl(Il2CppReflectionType* type)
    {
        const Il2CppGenericParameter* genericParameter = vm::Type::GetGenericParameter(type->type);
        if (genericParameter->ownerIndex == kGenericContainerIndexInvalid)
            return NULL;

        Il2CppArray* res = il2cpp_array_new(il2cpp_defaults.monotype_class, genericParameter->constraintsCount);
        for (int i = 0; i < genericParameter->constraintsCount; i++)
            il2cpp_array_setref(res, i, il2cpp_type_get_object(MetadataCache::GetGenericParameterConstraintFromIndex(genericParameter->constraintsStart + i)));

        return res;
    }

    void Type::GetPacking(Il2CppReflectionType* type, int32_t* packing, int32_t* size)
    {
        NOT_IMPLEMENTED_ICALL_NO_ASSERT(Type::GetPacking, "In progress, need this function to not assert to test other aspects of StructLayout attribute");
        *packing = 8;
        *size = 0;
    }
} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
