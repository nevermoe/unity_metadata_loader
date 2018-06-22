#if IL2CPP_DEBUGGER_ENABLED

#include "variant.h"

#include "data-buffer.h"
#include "../il2cpp-api.h"
#include "../blob.h"
#include "../tabledefs.h"
#include "../class-internals.h"
#include "../object-internals.h"
#include "vm/Type.h"

using namespace il2cpp::debugger;

#define VALUE_TYPE_ID_NULL  0xF0

Variant::Variant() :
    _type(0),
    _value(0)
{
}

Variant::Variant(const Il2CppType *type, void *value) :
    _type(type),
    _value(value)
{
}

void Variant::WriteTo(Buffer &out) const
{
    switch (_type->type)
    {
        case IL2CPP_TYPE_VOID:
            out.WriteByte(_type->type);
            break;

        case IL2CPP_TYPE_BOOLEAN:
        case IL2CPP_TYPE_I1:
        case IL2CPP_TYPE_U1:
            out.WriteByte(_type->type);
            out.WriteInt(*((int8_t*)_value));
            break;

        case IL2CPP_TYPE_CHAR:
        case IL2CPP_TYPE_I2:
        case IL2CPP_TYPE_U2:
            out.WriteByte(_type->type);
            out.WriteInt(*((int16_t*)_value));
            break;

        case IL2CPP_TYPE_I4:
        case IL2CPP_TYPE_U4:
        case IL2CPP_TYPE_R4:
            out.WriteByte(_type->type);
            out.WriteInt(*((int32_t*)_value));
            break;

        case IL2CPP_TYPE_I8:
        case IL2CPP_TYPE_U8:
        case IL2CPP_TYPE_R8:
            out.WriteByte(_type->type);
            out.WriteLong(*((int64_t*)_value));
            break;

        case IL2CPP_TYPE_I:
        case IL2CPP_TYPE_U:
            VTypeVariant(_type, _value).WriteTo(out);
            break;

        case IL2CPP_TYPE_PTR:
            out.WriteByte(_type->type);
            out.WriteLong(*((size_t*)_value));
            break;

        // handle_ref:
        case IL2CPP_TYPE_STRING:
        case IL2CPP_TYPE_SZARRAY:
        case IL2CPP_TYPE_OBJECT:
        case IL2CPP_TYPE_CLASS:
        case IL2CPP_TYPE_ARRAY:
        {
            Il2CppObject *obj = *(Il2CppObject**)_value;

            if (!obj)
            {
                out.WriteByte(VALUE_TYPE_ID_NULL);
            }
            else
            {
                if (obj->klass->valuetype)
                {
                    VTypeVariant(obj->klass->byval_arg, il2cpp_object_unbox(obj)).WriteTo(out);
                }
                else
                {
                    if (obj->klass->rank)
                        out.WriteByte(obj->klass->byval_arg->type);
                    else if (obj->klass->byval_arg->type == IL2CPP_TYPE_GENERICINST)
                        out.WriteByte(IL2CPP_TYPE_CLASS);
                    else
                        out.WriteByte(obj->klass->byval_arg->type);

                    out.WriteObjectId(obj);
                }
            }
        }
        break;

        case IL2CPP_TYPE_VALUETYPE:
            VTypeVariant(_type, _value).WriteTo(out);
            break;

        case IL2CPP_TYPE_GENERICINST:
            LOG("Encoding of generic instance types not supported yet.");
            IL2CPP_ASSERT(0 && "Variant::WriteTo: encoding of generic instance types not supported yet");
            break;

        default:
            IL2CPP_ASSERT(0 && "Variant::WriteTo: unexpected type");
            break;
    }
}

ErrorCode Variant::ReadInto(Buffer &in, const Il2CppType *expected_type, void *addr)
{
    // TODO: probably nullables need to be dealt with in a special way. Test it.
    int32_t type_code = in.ReadByte();

    switch (type_code)
    {
        case IL2CPP_TYPE_BOOLEAN:
            *(uint8_t*)addr = in.ReadInt();
            break;

        case IL2CPP_TYPE_CHAR:
            *(uint16_t*)addr = in.ReadInt();
            break;

        case IL2CPP_TYPE_I1:
            *(int8_t*)addr = in.ReadInt();
            break;

        case IL2CPP_TYPE_U1:
            *(uint8_t*)addr = in.ReadInt();
            break;

        case IL2CPP_TYPE_I2:
            *(int16_t*)addr = in.ReadInt();
            break;

        case IL2CPP_TYPE_U2:
            *(uint16_t*)addr = in.ReadInt();
            break;

        case IL2CPP_TYPE_I4:
            *(int32_t*)addr = in.ReadInt();
            break;

        case IL2CPP_TYPE_U4:
            *(uint32_t*)addr = in.ReadInt();
            break;

        case IL2CPP_TYPE_I8:
            *(int64_t*)addr = in.ReadLong();
            break;

        case IL2CPP_TYPE_U8:
            *(uint64_t*)addr = in.ReadLong();
            break;

        case IL2CPP_TYPE_R4:
            *(uint32_t*)addr = in.ReadInt();
            break;

        case IL2CPP_TYPE_R8:
            *(uint64_t*)addr = in.ReadLong();
            break;

        case IL2CPP_TYPE_PTR:
            *(size_t*)addr = in.ReadLong();
            break;

        case IL2CPP_TYPE_GENERICINST:
            /*if (IL2CPP_TYPE_ISSTRUCT (t))
                goto handle_vtype;
            else
                goto handle_ref;*/
            LOG("Decoding of generic instance types not supported yet.");
            IL2CPP_ASSERT(0 && "Variant::ReadFrom: encoding of generic instance types not supported yet");
            break;

        // handle_vtype:
        case IL2CPP_TYPE_I:
        case IL2CPP_TYPE_U:
        case IL2CPP_TYPE_VALUETYPE:
        {
            bool is_enum = in.ReadBool();

            IL2CPP_ASSERT(!is_enum && "Variant::ReadFrom: is_enum not implemented!");

            Il2CppClass *klass = in.ReadType();

            void *iter = 0;
            FieldInfo *field;
            int32_t fields_count = in.ReadInt();

            while ((field = il2cpp_class_get_fields(klass, &iter)))
            {
                if (field->type->attrs & FIELD_ATTRIBUTE_STATIC)
                    continue;

                /*if(il2cpp_field_is_deleted (field))
                    continue;*/

                Variant::ReadInto(in, field->type, (uint8_t*)addr + field->offset - sizeof(Il2CppObject));

                --fields_count;
            }

            IL2CPP_ASSERT(fields_count == 0 && "Variant::ReadFrom: some fields have not been decoded properly!");
        }
        break;

        // handle_ref:
        default:
        {
            IL2CPP_ASSERT(!vm::Type::IsValueType(expected_type) && "Variant::ReadFrom: expecting a reference type!");

            switch (type_code)
            {
                case IL2CPP_TYPE_OBJECT:
                {
                    Il2CppObject *obj = in.ReadObject();

                    if (obj && !il2cpp_class_is_assignable_from(il2cpp_class_from_il2cpp_type(expected_type), obj->klass))
                        return kErrorCodeInvalidArgument;

                    *(Il2CppObject**)addr = obj;
                }
                break;

                case VALUE_TYPE_ID_NULL:
                    *(Il2CppObject**)addr = NULL;
                    break;

                default:
                    IL2CPP_ASSERT(0 && "Variant::ReadFrom: unexpected type code!");
                    break;
            }
        }
        break;
    }

    return kErrorCodeNone;
}

VTypeVariant::VTypeVariant() :
    Variant(0, 0)
{
}

VTypeVariant::VTypeVariant(const Il2CppType *type, void *value) :
    Variant(type, value)
{
}

void VTypeVariant::WriteTo(Buffer &out) const
{
    switch (type()->type)
    {
        case IL2CPP_TYPE_BOOLEAN:
        case IL2CPP_TYPE_I1:
        case IL2CPP_TYPE_U1:
        case IL2CPP_TYPE_CHAR:
        case IL2CPP_TYPE_I2:
        case IL2CPP_TYPE_U2:
        case IL2CPP_TYPE_I4:
        case IL2CPP_TYPE_U4:
        case IL2CPP_TYPE_R4:
        case IL2CPP_TYPE_I8:
        case IL2CPP_TYPE_U8:
        case IL2CPP_TYPE_R8:
        case IL2CPP_TYPE_I:
        case IL2CPP_TYPE_U:
        case IL2CPP_TYPE_PTR:
        case IL2CPP_TYPE_VALUETYPE:
        {
            FieldInfo *field_info;
            Il2CppClass *klass = il2cpp_class_from_il2cpp_type(type());

            out.WriteByte(IL2CPP_TYPE_VALUETYPE);
            out.WriteByte(klass->enumtype);
            out.WriteTypeId(klass);

            size_t nfields = 0;
            void *iter = NULL;

            while ((field_info = il2cpp_class_get_fields(klass, &iter)))
            {
                if (field_info->type->attrs & FIELD_ATTRIBUTE_STATIC)
                    continue;

                /*if (il2cpp_field_is_deleted(field_info))
                    continue;*/

                nfields++;
            }

            out.WriteInt(nfields);

            iter = NULL;
            while ((field_info = il2cpp_class_get_fields(klass, &iter)))
            {
                if (field_info->type->attrs & FIELD_ATTRIBUTE_STATIC)
                    continue;

                /*if (il2cpp_field_is_deleted(field_info))
                    continue;*/

                Variant(field_info->type, (uint8_t*)value() + field_info->offset - sizeof(Il2CppObject)).WriteTo(out);
            }
        }
        break;

        default:
            Variant::WriteTo(out);
            break;
    }
}

#endif
