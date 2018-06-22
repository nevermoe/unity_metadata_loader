#include "il2cpp-config.h"
#include <memory>
#include "icalls/mscorlib/System/ValueType.h"
#include "tabledefs.h"
#include "class-internals.h"
#include "object-internals.h"
#include "gc/WriteBarrier.h"
#include "utils/StringUtils.h"
#include "vm/Array.h"
#include "vm/Class.h"
#include "vm/Exception.h"
#include "vm/Field.h"
#include "vm/Object.h"
#include "vm/String.h"

using namespace il2cpp::vm;
using il2cpp::gc::WriteBarrier;

namespace il2cpp
{
namespace icalls
{
namespace mscorlib
{
namespace System
{
    bool ValueType::InternalEquals(Il2CppObject * thisPtr, Il2CppObject * that, Il2CppArray** fields)
    {
        Il2CppClass *klass;
        Il2CppObject **values = NULL;
        Il2CppObject *o;
        FieldInfo* field;
        void* iter;
        int count = 0;

        IL2CPP_CHECK_ARG_NULL(that);

        if (thisPtr->klass != that->klass)
            return false;

        klass = Object::GetClass(thisPtr);

        if (klass->enumtype && Class::GetEnumBaseType(klass) && Class::GetEnumBaseType(klass)->type == IL2CPP_TYPE_I4)
            return (*(int32_t*)((uint8_t*)thisPtr + sizeof(Il2CppObject)) == *(int32_t*)((uint8_t*)that + sizeof(Il2CppObject)));

        /*
         * Do the comparison for fields of primitive type and return a result if
         * possible. Otherwise, return the remaining fields in an array to the
         * managed side. This way, we can avoid costly reflection operations in
         * managed code.
         */
        *fields = NULL;
        iter = NULL;
        while ((field = Class::GetFields(klass, &iter)))
        {
            if (field->type->attrs & FIELD_ATTRIBUTE_STATIC)
                continue;
            if (Field::IsDeleted(field))
                continue;
            /* FIXME: Add more types */

            switch (field->type->type)
            {
                case IL2CPP_TYPE_I1:
                case IL2CPP_TYPE_U1:
                case IL2CPP_TYPE_BOOLEAN:
                    if (*((uint8_t*)thisPtr + field->offset) != *((uint8_t*)that + field->offset))
                        return false;
                    break;
                case IL2CPP_TYPE_I2:
                case IL2CPP_TYPE_U2:
                case IL2CPP_TYPE_CHAR:
                    if (*(int16_t*)((uint8_t*)thisPtr + field->offset) != *(int16_t*)((uint8_t*)that + field->offset))
                        return false;
                    break;
                case IL2CPP_TYPE_I4:
                case IL2CPP_TYPE_U4:
                    if (*(int32_t*)((uint8_t*)thisPtr + field->offset) != *(int32_t*)((uint8_t*)that + field->offset))
                        return false;
                    break;
                case IL2CPP_TYPE_I8:
                case IL2CPP_TYPE_U8:
                    if (*(int64_t*)((uint8_t*)thisPtr + field->offset) != *(int64_t*)((uint8_t*)that + field->offset))
                        return false;
                    break;
                case IL2CPP_TYPE_R4:
                    if (*(float*)((uint8_t*)thisPtr + field->offset) != *(float*)((uint8_t*)that + field->offset))
                        return false;
                    break;
                case IL2CPP_TYPE_R8:
                    if (*(double*)((uint8_t*)thisPtr + field->offset) != *(double*)((uint8_t*)that + field->offset))
                        return false;
                    break;
                case IL2CPP_TYPE_STRING:
                    Il2CppString *s1, *s2;
                    uint32_t s1len, s2len;
                    s1 = *(Il2CppString**)((uint8_t*)thisPtr + field->offset);
                    s2 = *(Il2CppString**)((uint8_t*)that + field->offset);
                    if (s1 == s2)
                        break;
                    if ((s1 == NULL) || (s2 == NULL))
                        return false;
                    s1len = utils::StringUtils::GetLength(s1);
                    s2len = utils::StringUtils::GetLength(s2);
                    if (s1len != s2len)
                        return false;

                    if (memcmp(utils::StringUtils::GetChars(s1), utils::StringUtils::GetChars(s2), s1len * sizeof(Il2CppChar)) != 0)
                        return false;
                    break;
                default:
                    if (!values)
                        values = (Il2CppObject**)alloca(sizeof(Il2CppObject*) * (Class::GetNumFields(klass) * 2));
                    o = Field::GetValueObject(field, thisPtr);
                    values[count++] = o;
                    o = Field::GetValueObject(field, that);
                    values[count++] = o;
            }

            if (klass->enumtype)
                /* enums only have one non-static field */
                break;
        }

        if (values)
        {
            int i;
            WriteBarrier::GenericStore(fields, (Il2CppObject*)Array::New(il2cpp_defaults.object_class, count));
            for (i = 0; i < count; ++i)
                il2cpp_array_setref(*fields, i, values[i]);
            return false;
        }
        else
        {
            return true;
        }
    }

    int ValueType::InternalGetHashCode(Il2CppObject* obj, Il2CppArray** fields)
    {
        Il2CppObject **values = NULL;
        int count = 0;
        int32_t result = 0;
        FieldInfo* field = NULL;
        void* iter = NULL;

        Il2CppClass* klass = Object::GetClass(obj);

        if (Class::GetNumFields(klass) == 0)
            return Object::GetHash(obj);

        /*
         * Compute the starting value of the hashcode for fields of primitive
         * types, and return the remaining fields in an array to the managed side.
         * This way, we can avoid costly reflection operations in managed code.
         */
        iter = NULL;
        while ((field = Class::GetFields(klass, &iter)))
        {
            if (field->type->attrs & FIELD_ATTRIBUTE_STATIC)
                continue;
            if (Field::IsDeleted(field))
                continue;
            /* FIXME: Add more types */
            switch (field->type->type)
            {
                case IL2CPP_TYPE_I4:
                    result ^= *(int32_t*)((uint8_t*)obj + field->offset);
                    break;
                case IL2CPP_TYPE_STRING:
                {
                    Il2CppString *s;
                    s = *(Il2CppString**)((uint8_t*)obj + field->offset);
                    if (s != NULL)
                        result ^= String::GetHash(s);
                    break;
                }
                default:
                    if (!values)
                        values = (Il2CppObject**)alloca(sizeof(Il2CppObject*) * Class::GetNumFields(klass));
                    Il2CppObject* o = Field::GetValueObject(field, obj);
                    values[count++] = o;
            }
        }

        if (values)
        {
            int i;
            WriteBarrier::GenericStore(fields, (Il2CppObject*)Array::New(il2cpp_defaults.object_class, count));
            for (i = 0; i < count; ++i)
                il2cpp_array_setref(*fields, i, values[i]);
        }
        else
        {
            *fields = NULL;
        }

        return result;
    }
} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
