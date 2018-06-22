#pragma once

#include "common.h"

#include <string>
#include <iterator>

struct Il2CppClass;

namespace il2cpp
{
namespace debugger
{

struct Id;
class Variant;

class Buffer
{

public:

	static const int32_t GrowStep = 32;

	explicit Buffer(size_t size);
	~Buffer();
	
	void Seek(int32_t offset);
	void Reset();
	void Reset(size_t size);
	void AssureRoomFor(size_t size);

	void WriteInt(uint32_t value);
	void WriteData(const uint8_t *data, size_t size);
	void WriteData(const Buffer &data);
	void WriteString(const char *value);
	void WriteString(const std::string &value);
	void WriteBool(bool value);
	void WriteByte(uint8_t value);
	void WritePtrId(const Id &id);
	void WriteObjectId(const Il2CppObject *object);
	void WriteThreadId(const Il2CppThread *thread);
	void WriteAssemblyId(const Il2CppAssembly *assembly);
	void WriteMethodId(const MethodInfo *method);
	void WriteFieldId(const FieldInfo *field);
	void WritePropertyId(const PropertyInfo *prop);
	void WriteModuleId(const Il2CppImage *image);
	void WriteTypeId(const Il2CppClass *klass);
	void WriteDomainId(const Il2CppDomain *domain);
	void WriteLong(uint64_t value);
	void WriteVariant(const Variant &value);
	
	int32_t ReadInt();
	uint8_t ReadByte();
	bool ReadBool();
	Il2CppClass *ReadType();
	Il2CppDomain *ReadDomain();
	Il2CppObject *ReadObject();
	Il2CppAssembly *ReadAssembly();
	Il2CppImage *ReadModule();
	MethodInfo *ReadMethod();
	FieldInfo *ReadField();
	PropertyInfo *ReadProperty();
	std::string ReadString();
	uint64_t ReadLong();
	Il2CppThread *ReadThread();

	void MarkReadRaw(size_t size);

	template<typename T>
	inline T ReadEnum()
	{
		return (T)ReadByte();
	}

	inline const int32_t position() const
	{
		return (((uintptr_t)_ptr) - ((uintptr_t)_data));
	}

	inline const size_t length() const
	{
		return (((uintptr_t)_tail) - ((uintptr_t)_data));
	}

	inline const size_t buffer_size() const
	{
		return (((uintptr_t)_end) - ((uintptr_t)_data));
	}

	inline uint8_t *raw()
	{
		return _data;
	}

	inline const uint8_t *raw() const
	{
		return _data;
	}

private:

	uint8_t *_data;
	uint8_t *_ptr;
	uint8_t *_tail;
	uint8_t *_end;

	DISALLOW_COPY(Buffer);
};

} /* namespace debugger */
} /* namespace il2cpp */
