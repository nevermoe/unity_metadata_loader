#include "il2cpp-config.h"
#include <memory>
#include "object-internals.h"
#include "class-internals.h"
#include "icalls/mscorlib/System/Array.h"
#include "utils/Exception.h"
#include "vm/Array.h"
#include "vm/Class.h"
#include "vm/Exception.h"
#include "vm/Object.h"
#include "vm/Type.h"

#include <vector>
#include <sstream>

using namespace il2cpp::vm;

namespace il2cpp
{
namespace icalls
{
namespace mscorlib
{
namespace System
{
    void Array::ClearInternal(Il2CppArray * arr, int32_t idx, int32_t length)
    {
        int sz = il2cpp_array_element_size(arr->klass);
        memset(il2cpp_array_addr_with_size(arr, sz, idx), 0, length * sz);
    }

    Il2CppArray * Array::Clone(Il2CppArray * arr)
    {
        Il2CppClass *typeInfo = arr->klass;
        const uint32_t elem_size = il2cpp::vm::Array::GetElementSize(typeInfo);

        if (arr->bounds == NULL)
        {
            il2cpp_array_size_t len = il2cpp::vm::Array::GetLength(arr);
            Il2CppArray *clone = (Il2CppArray*)il2cpp::vm::Array::NewFull(typeInfo, &len, NULL);
            memcpy(il2cpp::vm::Array::GetFirstElementAddress(clone), il2cpp::vm::Array::GetFirstElementAddress(arr), elem_size * len);

            return clone;
        }

        il2cpp_array_size_t size = elem_size;
        std::vector<il2cpp_array_size_t> lengths(typeInfo->rank);
        std::vector<il2cpp_array_size_t> lowerBounds(typeInfo->rank);

        for (int i = 0; i < typeInfo->rank; ++i)
        {
            lengths[i] = arr->bounds[i].length;
            size *= arr->bounds[i].length;
            lowerBounds[i] = arr->bounds[i].lower_bound;
        }

        Il2CppArray* clone = il2cpp::vm::Array::NewFull(typeInfo, &lengths[0], &lowerBounds[0]);
        memcpy(il2cpp::vm::Array::GetFirstElementAddress(clone), il2cpp::vm::Array::GetFirstElementAddress(arr), size);

        return clone;
    }

    static std::string FormatCreateInstanceException(const Il2CppType* type)
    {
        std::string typeName = vm::Type::GetName(type, IL2CPP_TYPE_NAME_FORMAT_IL);

        std::string message;
        message += "Unable to create an array of type '";
        message += typeName;
        message += "'. IL2CPP needs to know about the array type at compile time, so please define a private static field like this:\n\nprivate static ";
        message += typeName;
        message += "[] _unused;\n\nin any MonoBehaviour class, and this exception should go away.";

        return message;
    }

    Il2CppArray * Array::CreateInstanceImpl(Il2CppReflectionType * elementType, Il2CppArray* lengths, Il2CppArray* bounds)
    {
        il2cpp_array_size_t* i32lengths = NULL;
        il2cpp_array_size_t* i32bounds = NULL;
        if (lengths != NULL)
            i32lengths = (il2cpp_array_size_t*)il2cpp_array_addr_with_size(lengths, il2cpp_array_element_size(lengths->klass), 0);

        if (bounds != NULL)
            i32bounds = (il2cpp_array_size_t*)il2cpp_array_addr_with_size(bounds, il2cpp_array_element_size(bounds->klass), 0);

        Il2CppClass* arrayType = il2cpp::vm::Class::GetArrayClassCached(il2cpp::vm::Class::FromIl2CppType(elementType->type), il2cpp::vm::Array::GetLength(lengths));

        if (arrayType == NULL)
            vm::Exception::Raise(vm::Exception::GetInvalidOperationException(FormatCreateInstanceException(elementType->type).c_str()));

        return (Il2CppArray*)il2cpp::vm::Array::NewFull(arrayType, i32lengths, i32bounds);
    }

    bool Array::FastCopy(Il2CppArray *source, int32_t source_idx, Il2CppArray *dest, int32_t dest_idx, int32_t length)
    {
        int element_size;
        Il2CppClass *src_class;
        Il2CppClass *dest_class;
        int i;

        if (source->klass->rank != dest->klass->rank)
            return false;

        if (source->bounds || dest->bounds)
            return false;

        // Our max array length is il2cpp_array_size_t, which is currently int32_t,
        // so Array::GetLength will never return more than 2^31 - 1
        // Therefore, casting sum to uint32_t is safe even if it overflows - it if does,
        // the comparison will succeed and this function will return false
        if ((static_cast<uint32_t>(dest_idx + length) > il2cpp::vm::Array::GetLength(dest)) ||
            (static_cast<uint32_t>(source_idx + length) > il2cpp::vm::Array::GetLength(source)))
            return false;

        src_class = source->klass->element_class;
        dest_class = dest->klass->element_class;

        // object[] -> valuetype[]
        if (src_class == il2cpp_defaults.object_class && dest_class->valuetype)
        {
            for (i = source_idx; i < source_idx + length; ++i)
            {
                Il2CppObject *elem = il2cpp_array_get(source, Il2CppObject*, i);
                if (elem && !Object::IsInst(elem, dest_class))
                    return false;
            }

            element_size = il2cpp_array_element_size(dest->klass);
            memset(il2cpp_array_addr_with_size(dest, element_size, dest_idx), 0, element_size * length);
            for (i = 0; i < length; ++i)
            {
                Il2CppObject *elem = il2cpp_array_get(source, Il2CppObject*, source_idx + i);
#if IL2CPP_ENABLE_MONO_BUG_EMULATION
                if (!elem)
                    continue;
#else
                if (!elem)
                    Exception::Raise(Exception::GetInvalidCastException("At least one element in the source array could not be cast down to the destination array type."));
#endif

                NOT_IMPLEMENTED_ICALL_NO_ASSERT(Array::FastCopy, "Need GC write barrier");
                memcpy(il2cpp_array_addr_with_size(dest, element_size, dest_idx + i), Object::Unbox(elem), element_size);
            }
            return true;
        }

        if (src_class != dest_class)
        {
            if (Class::IsValuetype(dest_class) || Class::IsEnum(dest_class) || Class::IsValuetype(src_class) || Class::IsEnum(src_class))
                return false;

            // object[] -> reftype[]
            if (Class::IsSubclassOf(dest_class, src_class, false))
            {
                for (i = source_idx; i < source_idx + length; ++i)
                {
                    Il2CppObject *elem = il2cpp_array_get(source, Il2CppObject*, i);
                    if (elem && !Object::IsInst(elem, dest_class))
                        Exception::Raise(Exception::GetInvalidCastException("At least one element in the source array could not be cast down to the destination array type."));
                }
            }
            else if (!Class::IsSubclassOf(src_class, dest_class, false))
                return false;

            // derivedtype[] -> basetype[]
            IL2CPP_ASSERT(Type::IsReference(src_class->byval_arg));
            IL2CPP_ASSERT(Type::IsReference(dest_class->byval_arg));
        }

        IL2CPP_ASSERT(il2cpp_array_element_size(dest->klass) == il2cpp_array_element_size(source->klass));

        NOT_IMPLEMENTED_ICALL_NO_ASSERT(Array::FastCopy, "Need GC write barrier");
        memmove(
            il2cpp_array_addr_with_size(dest, il2cpp_array_element_size(dest->klass), dest_idx),
            il2cpp_array_addr_with_size(source, il2cpp_array_element_size(source->klass), source_idx),
            length * il2cpp_array_element_size(dest->klass));

        return true;
    }

    int32_t Array::GetLength(Il2CppArray * thisPtr, int dimension)
    {
        int32_t rank = thisPtr->klass->rank;
        il2cpp_array_size_t length;

        if ((dimension < 0) || (dimension >= rank))
            il2cpp::vm::Exception::Raise(il2cpp::vm::Exception::GetIndexOutOfRangeException());

        if (thisPtr->bounds == NULL)
            length = thisPtr->max_length;
        else
            length = thisPtr->bounds[dimension].length;

#ifdef IL2CPP_BIG_ARRAYS
        if (length > G_MAXINT32)
            mono_raise_exception(mono_get_exception_overflow());
#endif
        return length;
    }

    int32_t Array::GetLowerBound(Il2CppArray * thisPtr, int32_t dimension)
    {
        int32_t rank = thisPtr->klass->rank;

        if ((dimension < 0) || (dimension >= rank))
            Exception::Raise(Exception::GetIndexOutOfRangeException());

        if (thisPtr->bounds == NULL)
            return false;

        return thisPtr->bounds[dimension].lower_bound;
    }

    int Array::GetRank(Il2CppArray * arr)
    {
        return arr->klass->rank;
    }

    Il2CppObject * Array::GetValue(Il2CppArray * thisPtr, Il2CppArray* indices)
    {
        Il2CppClass *ac, *ic;
        Il2CppArray *ao, *io;
        int32_t i, pos, *ind;

        IL2CPP_CHECK_ARG_NULL(indices);

        io = (Il2CppArray*)indices;
        ic = (Il2CppClass*)io->klass;

        ao = (Il2CppArray*)thisPtr;
        ac = (Il2CppClass*)ao->klass;

        IL2CPP_ASSERT(ic->rank == 1);
        if (io->bounds != NULL || io->max_length !=  ac->rank)
            Exception::Raise(Exception::GetArgumentException(NULL, NULL));

        ind = (int32_t*)il2cpp::vm::Array::GetFirstElementAddress(io);

        if (ao->bounds == NULL)
        {
            if (*ind < 0 || *ind >= ao->max_length)
                Exception::Raise(Exception::GetIndexOutOfRangeException());

            return GetValueImpl(thisPtr, *ind);
        }

        for (i = 0; i < ac->rank; i++)
            if ((ind[i] < ao->bounds[i].lower_bound) ||
                (ind[i] >= ao->bounds[i].length + ao->bounds[i].lower_bound))
                Exception::Raise(Exception::GetIndexOutOfRangeException());

        pos = ind[0] - ao->bounds[0].lower_bound;
        for (i = 1; i < ac->rank; i++)
            pos = pos * ao->bounds[i].length + ind[i] - ao->bounds[i].lower_bound;

        return GetValueImpl(thisPtr, pos);
    }

    Il2CppObject * Array::GetValueImpl(Il2CppArray * thisPtr, int32_t pos)
    {
        Il2CppClass* typeInfo = thisPtr->klass;
        void **ea;

        ea = (void**)load_array_elema(thisPtr, pos, typeInfo->element_size);

        if (typeInfo->element_class->valuetype)
            return il2cpp::vm::Object::Box(typeInfo->element_class, ea);

        return (Il2CppObject*)*ea;
    }

    void Array::SetValue(Il2CppArray * thisPtr, Il2CppObject* value, Il2CppArray* idxs)
    {
        Il2CppClass *ac, *ic;
        int32_t i, pos, *ind;

        IL2CPP_CHECK_ARG_NULL(idxs);

        ic = idxs->klass;
        ac = thisPtr->klass;

        IL2CPP_ASSERT(ic->rank == 1);
        if (idxs->bounds != NULL || idxs->max_length != ac->rank)
            Exception::Raise(Exception::GetArgumentException(NULL, NULL));

        ind = (int32_t*)il2cpp::vm::Array::GetFirstElementAddress(idxs);

        if (thisPtr->bounds == NULL)
        {
            if (*ind < 0 || *ind >= thisPtr->max_length)
                Exception::Raise(Exception::GetIndexOutOfRangeException());

            SetValueImpl(thisPtr, value, *ind);
            return;
        }

        for (i = 0; i < ac->rank; i++)
            if ((ind[i] < thisPtr->bounds[i].lower_bound) ||
                (ind[i] >= (il2cpp_array_lower_bound_t)thisPtr->bounds[i].length + thisPtr->bounds[i].lower_bound))
                Exception::Raise(Exception::GetIndexOutOfRangeException());

        pos = ind[0] - thisPtr->bounds[0].lower_bound;
        for (i = 1; i < ac->rank; i++)
            pos = pos * thisPtr->bounds[i].length + ind[i] -
                thisPtr->bounds[i].lower_bound;

        SetValueImpl(thisPtr, value, pos);
    }

    static void ThrowNoWidening()
    {
        Exception::Raise(Exception::GetArgumentException("value", "not a widening conversion"));
    }

    static void ThrowInvalidCast(const Il2CppClass* a, const Il2CppClass* b)
    {
        Exception::Raise(Exception::GetInvalidCastException(utils::Exception::FormatInvalidCastException(b, a).c_str()));
    }

    union WidenedValueUnion
    {
        int64_t i64;
        uint64_t u64;
        double r64;
    };

    WidenedValueUnion ExtractWidenedValue(Il2CppTypeEnum type, void* value)
    {
        WidenedValueUnion extractedValue = { 0 };
        switch (type)
        {
            case IL2CPP_TYPE_U1:
                extractedValue.u64 = *(uint8_t*)value;
                break;
            case IL2CPP_TYPE_CHAR:
                extractedValue.u64 = *(Il2CppChar*)value;
                break;
            case IL2CPP_TYPE_U2:
                extractedValue.u64 = *(uint16_t*)value;
                break;
            case IL2CPP_TYPE_U4:
                extractedValue.u64 = *(uint32_t*)value;
                break;
            case IL2CPP_TYPE_U8:
                extractedValue.u64 = *(uint64_t*)value;
                break;
            case IL2CPP_TYPE_I1:
                extractedValue.i64 = *(int8_t*)value;
                break;
            case IL2CPP_TYPE_I2:
                extractedValue.i64 = *(int16_t*)value;
                break;
            case IL2CPP_TYPE_I4:
                extractedValue.i64 = *(int32_t*)value;
                break;
            case IL2CPP_TYPE_I8:
                extractedValue.i64 = *(int64_t*)value;
                break;
            case IL2CPP_TYPE_R4:
                extractedValue.r64 = *(float*)value;
                break;
            case IL2CPP_TYPE_R8:
                extractedValue.r64 = *(double*)value;
                break;
            default:
                IL2CPP_ASSERT(0);
                break;
        }

        return extractedValue;
    }

    static void CheckWideningConversion(size_t elementSize, size_t valueSize, size_t extra = 0)
    {
        if (elementSize < valueSize + (extra))
            ThrowNoWidening();
    }

    template<typename T>
    static void AssignUnsigned(WidenedValueUnion value, void* elementAddress, Il2CppTypeEnum valueType, size_t elementSize, size_t valueSize)
    {
        switch (valueType)
        {
            case IL2CPP_TYPE_U1:
            case IL2CPP_TYPE_U2:
            case IL2CPP_TYPE_U4:
            case IL2CPP_TYPE_U8:
            case IL2CPP_TYPE_CHAR:
                CheckWideningConversion(elementSize, valueSize);
                *(T*)elementAddress = (T)value.u64;
                break;
            /* You can't assign a signed value to an unsigned array. */
            case IL2CPP_TYPE_I1:
            case IL2CPP_TYPE_I2:
            case IL2CPP_TYPE_I4:
            case IL2CPP_TYPE_I8:
            /* You can't assign a floating point number to an integer array. */
            case IL2CPP_TYPE_R4:
            case IL2CPP_TYPE_R8:
                ThrowNoWidening();
                break;
            default:
                IL2CPP_ASSERT(0);
                break;
        }
    }

    template<typename T>
    static void AssignSigned(WidenedValueUnion value, void* elementAddress, Il2CppTypeEnum valueType, size_t elementSize, size_t valueSize)
    {
        switch (valueType)
        {
            /* You can assign an unsigned value to a signed array if the array's
                element size is larger than the value size. */
            case IL2CPP_TYPE_U1:
            case IL2CPP_TYPE_U2:
            case IL2CPP_TYPE_U4:
            case IL2CPP_TYPE_U8:
            case IL2CPP_TYPE_CHAR:
                CheckWideningConversion(elementSize, valueSize, 1);
                *(T*)elementAddress = (T)value.u64;
                break;
            case IL2CPP_TYPE_I1:
            case IL2CPP_TYPE_I2:
            case IL2CPP_TYPE_I4:
            case IL2CPP_TYPE_I8:
                CheckWideningConversion(elementSize, valueSize);
                *(T*)elementAddress = (T)value.i64;
                break;
            /* You can't assign a floating point number to an integer array. */
            case IL2CPP_TYPE_R4:
            case IL2CPP_TYPE_R8:
                ThrowNoWidening();
                break;
            default:
                IL2CPP_ASSERT(0);
                break;
        }
    }

    template<typename T>
    static void AssignReal(WidenedValueUnion value, void* elementAddress, Il2CppTypeEnum valueType, size_t elementSize, size_t valueSize)
    {
        switch (valueType)
        {
            /* All integers fit into the floating point value range. No need to check size. */
            case IL2CPP_TYPE_U1:
            case IL2CPP_TYPE_U2:
            case IL2CPP_TYPE_U4:
            case IL2CPP_TYPE_U8:
            case IL2CPP_TYPE_CHAR:
                *(T*)elementAddress = (T)value.u64;
                break;
            case IL2CPP_TYPE_I1:
            case IL2CPP_TYPE_I2:
            case IL2CPP_TYPE_I4:
            case IL2CPP_TYPE_I8:
                *(T*)elementAddress = (T)value.i64;
                break;
            case IL2CPP_TYPE_R4:
            case IL2CPP_TYPE_R8:
                CheckWideningConversion(elementSize, valueSize);
                *(T*)elementAddress = (T)value.r64;
                break;
            default:
                IL2CPP_ASSERT(0);
                break;
        }
    }

    static void NullableInit(uint8_t* buf, Il2CppObject* value, Il2CppClass* klass)
    {
        Il2CppClass *parameterClass = klass->castClass;

        IL2CPP_ASSERT(Class::FromIl2CppType(klass->fields[0].type) == parameterClass);
        IL2CPP_ASSERT(Class::FromIl2CppType(klass->fields[1].type) == il2cpp_defaults.boolean_class);

        *(uint8_t*)(buf + klass->fields[1].offset - sizeof(Il2CppObject)) = value ? 1 : 0;
        if (value)
            memcpy(buf + klass->fields[0].offset - sizeof(Il2CppObject), Object::Unbox(value), Class::GetValueSize(parameterClass, NULL));
        else
            memset(buf + klass->fields[0].offset - sizeof(Il2CppObject), 0, Class::GetValueSize(parameterClass, NULL));
    }

    void Array::SetValueImpl(Il2CppArray * thisPtr, Il2CppObject * value, int index)
    {
        Il2CppClass* typeInfo = thisPtr->klass;
        Il2CppClass* elementClass = Class::GetElementClass(typeInfo);

        int elementSize = Class::GetArrayElementSize(elementClass);
        void* elementAddress = il2cpp_array_addr_with_size(thisPtr, elementSize, index);

        if (Class::IsNullable(elementClass))
        {
            NullableInit((uint8_t*)elementAddress, value, elementClass);
            return;
        }

        if (value == NULL)
        {
            memset(elementAddress, 0, elementSize);
            return;
        }

        if (!Class::IsValuetype(elementClass))
        {
            if (!Object::IsInst(value, elementClass))
                Exception::Raise(Exception::GetInvalidCastException(utils::Exception::FormatInvalidCastException(thisPtr->klass->element_class, value->klass).c_str()));
            il2cpp_array_setref(thisPtr, index, value);
            return;
        }

        if (Object::IsInst(value, elementClass))
        {
            // write barrier
            memcpy(elementAddress, Object::Unbox(value), elementSize);
            return;
        }

        Il2CppClass* valueClass = Object::GetClass(value);

        if (!Class::IsValuetype(valueClass))
            ThrowInvalidCast(elementClass, valueClass);

        int valueSize = Class::GetInstanceSize(valueClass) - sizeof(Il2CppObject);

        Il2CppTypeEnum elementType = Class::IsEnum(elementClass) ? Class::GetEnumBaseType(elementClass)->type : elementClass->byval_arg->type;
        Il2CppTypeEnum valueType = Class::IsEnum(valueClass) ? Class::GetEnumBaseType(valueClass)->type : valueClass->byval_arg->type;

        if (elementType == IL2CPP_TYPE_BOOLEAN)
        {
            switch (valueType)
            {
                case IL2CPP_TYPE_BOOLEAN:
                    break;
                case IL2CPP_TYPE_CHAR:
                case IL2CPP_TYPE_U1:
                case IL2CPP_TYPE_U2:
                case IL2CPP_TYPE_U4:
                case IL2CPP_TYPE_U8:
                case IL2CPP_TYPE_I1:
                case IL2CPP_TYPE_I2:
                case IL2CPP_TYPE_I4:
                case IL2CPP_TYPE_I8:
                case IL2CPP_TYPE_R4:
                case IL2CPP_TYPE_R8:
                    ThrowNoWidening();
                default:
                    ThrowInvalidCast(elementClass, valueClass);
            }
        }

        WidenedValueUnion widenedValue = ExtractWidenedValue(valueType, Object::Unbox(value));

        switch (elementType)
        {
            case IL2CPP_TYPE_U1:
                AssignUnsigned<uint8_t>(widenedValue, elementAddress, valueType, elementSize, valueSize);
                break;
            case IL2CPP_TYPE_CHAR:
                AssignUnsigned<Il2CppChar>(widenedValue, elementAddress, valueType, elementSize, valueSize);
                break;
            case IL2CPP_TYPE_U2:
                AssignUnsigned<uint16_t>(widenedValue, elementAddress, valueType, elementSize, valueSize);
                break;
            case IL2CPP_TYPE_U4:
                AssignUnsigned<uint32_t>(widenedValue, elementAddress, valueType, elementSize, valueSize);
                break;
            case IL2CPP_TYPE_U8:
                AssignUnsigned<uint64_t>(widenedValue, elementAddress, valueType, elementSize, valueSize);
                break;
            case IL2CPP_TYPE_I1:
                AssignSigned<int8_t>(widenedValue, elementAddress, valueType, elementSize, valueSize);
                break;
            case IL2CPP_TYPE_I2:
                AssignSigned<int16_t>(widenedValue, elementAddress, valueType, elementSize, valueSize);
                break;
            case IL2CPP_TYPE_I4:
                AssignSigned<int32_t>(widenedValue, elementAddress, valueType, elementSize, valueSize);
                break;
            case IL2CPP_TYPE_I8:
                AssignSigned<int64_t>(widenedValue, elementAddress, valueType, elementSize, valueSize);
                break;
            case IL2CPP_TYPE_R4:
                AssignReal<float>(widenedValue, elementAddress, valueType, elementSize, valueSize);
                break;
            case IL2CPP_TYPE_R8:
                AssignReal<double>(widenedValue, elementAddress, valueType, elementSize, valueSize);
                break;
            default:
                ThrowInvalidCast(elementClass, valueClass);
                break;
        }
    }
} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
