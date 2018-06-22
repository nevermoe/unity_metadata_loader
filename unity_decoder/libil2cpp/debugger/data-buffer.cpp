#if IL2CPP_DEBUGGER_ENABLED

#include "data-buffer.h"

#include "common.h"
#include "variant.h"

#include "id-bank.h"

using namespace il2cpp::debugger;

Buffer::Buffer(size_t size) :
    _data(0),
    _ptr(0),
    _tail(0),
    _end(0)
{
    AssureRoomFor(size);

    memset(_data, 0, buffer_size());
}

Buffer::~Buffer()
{
    if (_data != 0)
    {
        delete[] _data;

        _data = 0;
        _ptr = 0;
        _tail = 0;
        _end = 0;
    }
}

void Buffer::Reset()
{
    memset(_data, 0, buffer_size());

    _ptr = _data;
    _tail = _data;
}

void Buffer::Reset(size_t sz)
{
    if (sz > buffer_size())
    {
        delete[] _data;

        _data = new uint8_t[((sz / GrowStep) + 1) * GrowStep];
        _ptr = _data;
        _tail = _data;
        _end = _data + sz;
    }

    Reset();
}

void Buffer::Seek(int32_t offset)
{
    const int32_t dist = (offset - (int32_t)buffer_size());
    if (dist > 0)
        AssureRoomFor(dist);

    _ptr = _data + offset;

    if (_ptr > _tail)
        _tail = _ptr;
}

void Buffer::AssureRoomFor(size_t size)
{
    const size_t pos = position();
    const size_t new_size = buffer_size() + size;
    const size_t tail_pos = length();

    if (pos + size <= buffer_size())
        return;

    uint8_t *new_data = new uint8_t[((new_size / GrowStep) + 1) * GrowStep];

    if (_data != 0)
    {
        memcpy(new_data, _data, buffer_size());
        delete[] _data;
    }

    _data = new_data;
    _ptr = _data + pos;
    _tail = _data + tail_pos;
    _end = _data + new_size;
}

void Buffer::WriteInt(uint32_t value)
{
    AssureRoomFor(4);

    *(_ptr + 0) = (value >> 24) & 0xFF;
    *(_ptr + 1) = (value >> 16) & 0xFF;
    *(_ptr + 2) = (value >> 8) & 0xFF;
    *(_ptr + 3) = (value >> 0) & 0xFF;

    _ptr += 4;

    if (_ptr > _tail)
        _tail = _ptr;
}

void Buffer::WriteString(const char *value)
{
    if (value != 0)
    {
        const size_t len = strlen(value);

        WriteInt(len);
        WriteData((uint8_t*)value, len);
    }
    else
    {
        WriteInt(0);
    }
}

void Buffer::WriteString(const std::string &value)
{
    WriteInt(value.size());
    WriteData((uint8_t*)value.c_str(), value.size());
}

void Buffer::WriteData(const uint8_t *data, size_t size)
{
    AssureRoomFor(size);
    std::copy(data, data + size, _ptr);

    _ptr += size;

    if (_ptr > _tail)
        _tail = _ptr;
}

void Buffer::WriteData(const Buffer &data)
{
    WriteData(data.raw(), data.length());
}

void Buffer::WriteByte(uint8_t value)
{
    AssureRoomFor(1);

    *_ptr = value;

    ++_ptr;

    if (_ptr > _tail)
        _tail = _ptr;
}

void Buffer::WriteBool(bool value)
{
    WriteByte((uint8_t)value);
}

void Buffer::WriteLong(uint64_t value)
{
    WriteInt((value >> 32) & 0xffffffff);
    WriteInt((value >> 0) & 0xffffffff);
}

void Buffer::WritePtrId(const Id &id)
{
    WriteInt(id.id);
}

void Buffer::WriteAssemblyId(const Il2CppAssembly *assembly)
{
    WritePtrId(AssemblyId(assembly));
}

void Buffer::WriteObjectId(const Il2CppObject *object)
{
    WritePtrId(ObjectId(object));
}

void Buffer::WriteThreadId(const Il2CppThread *thread)
{
    WritePtrId(ObjectId((Il2CppObject*)thread));
}

void Buffer::WriteModuleId(const Il2CppImage *image)
{
    WritePtrId(ModuleId(image));
}

void Buffer::WriteTypeId(const Il2CppClass *klass)
{
    WritePtrId(TypeId(klass));
}

void Buffer::WriteDomainId(const Il2CppDomain *domain)
{
    WritePtrId(DomainId(domain));
}

void Buffer::WriteMethodId(const MethodInfo *method)
{
    WritePtrId(MethodId(method));
}

void Buffer::WriteFieldId(const FieldInfo *field)
{
    WritePtrId(FieldId(field));
}

void Buffer::WritePropertyId(const PropertyInfo *prop)
{
    WritePtrId(PropertyId(prop));
}

void Buffer::WriteVariant(const Variant &value)
{
    value.WriteTo(*this);
}

int32_t Buffer::ReadInt()
{
    IL2CPP_ASSERT(position() + 4 <= length() && "Cannot read past the end of the buffer!");

    uint8_t *c = _ptr;

    _ptr += 4;

    if (_ptr > _tail)
        _tail = _ptr;

    return (((int32_t)c[0]) << 24)
        | (((int32_t)c[1]) << 16)
        | (((int32_t)c[2]) << 8)
        | (((int32_t)c[3]) << 0);
}

uint8_t Buffer::ReadByte()
{
    IL2CPP_ASSERT(position() + 1 <= length() && "Cannot read past the end of the buffer!");

    uint8_t *c = _ptr;

    _ptr += 1;

    if (_ptr > _tail)
        _tail = _ptr;

    return (uint8_t)c[0];
}

bool Buffer::ReadBool()
{
    return ReadByte() ? true : false;
}

Il2CppClass *Buffer::ReadType()
{
    return TypeFromId(ReadInt());
}

Il2CppObject *Buffer::ReadObject()
{
    Id id;
    id.type = kIdTypeObject;
    id.id = ReadInt();

    return ObjectFromId(id);
}

Il2CppThread *Buffer::ReadThread()
{
    Id id;
    id.type = kIdTypeObject;
    id.id = ReadInt();

    return (Il2CppThread*)ObjectFromId(id);
}

uint64_t Buffer::ReadLong()
{
    uint32_t high = ReadInt();
    uint32_t low = ReadInt();

    return ((((uint64_t)high) << 32) | ((uint64_t)low));
}

Il2CppDomain *Buffer::ReadDomain()
{
    Id id;
    id.type = kIdTypeDomain;
    id.id = ReadInt();

    return DomainFromId(id);
}

Il2CppAssembly *Buffer::ReadAssembly()
{
    Id id;
    id.type = kIdTypeAssembly;
    id.id = ReadInt();

    return AssemblyFromId(id);
}

Il2CppImage *Buffer::ReadModule()
{
    Id id;
    id.type = kIdTypeModule;
    id.id = ReadInt();

    return ModuleFromId(id);
}

MethodInfo *Buffer::ReadMethod()
{
    Id id;
    id.type = kIdTypeMethod;
    id.id = ReadInt();

    return MethodFromId(id);
}

FieldInfo *Buffer::ReadField()
{
    Id id;
    id.type = kIdTypeField;
    id.id = ReadInt();

    return FieldFromId(id);
}

PropertyInfo *Buffer::ReadProperty()
{
    Id id;
    id.type = kIdTypeProperty;
    id.id = ReadInt();

    return PropertyFromId(id);
}

std::string Buffer::ReadString()
{
    const size_t len = ReadInt();

    std::string value;
    value.resize(len);

    memcpy(&value[0], _ptr, len);

    _ptr += len;

    return value;
}

void Buffer::MarkReadRaw(size_t size)
{
    IL2CPP_ASSERT(size <= buffer_size() && "A raw read was performed, but too many bytes were read. Memory overwrite!");

    _tail += size;
}

#endif
