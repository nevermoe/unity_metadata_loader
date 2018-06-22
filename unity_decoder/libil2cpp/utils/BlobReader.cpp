#include "il2cpp-config.h"
#include <stdint.h>
#include <cassert>
#include "BlobReader.h"
#include "MemoryRead.h"
#include "../vm/String.h"
#include "object-internals.h"

namespace il2cpp
{
namespace utils
{

int BlobReader::GetConstantValueFromBlob(Il2CppTypeEnum type, const char *blob, void *value)
{
	int retval = 0;
	const char *p = blob;

	switch (type) {
	case IL2CPP_TYPE_BOOLEAN:
	case IL2CPP_TYPE_U1:
	case IL2CPP_TYPE_I1:
		*(uint8_t *)value = *p;
		break;
	case IL2CPP_TYPE_CHAR:
		*(Il2CppChar*)value = ReadChar(p);
		break;
	case IL2CPP_TYPE_U2:
	case IL2CPP_TYPE_I2:
		*(uint16_t *)value = Read16(p);
		break;
	case IL2CPP_TYPE_U4:
	case IL2CPP_TYPE_I4:
		*(uint32_t *)value = Read32(p);
		break;
	case IL2CPP_TYPE_U8:
	case IL2CPP_TYPE_I8:
		*(uint64_t *)value = Read64(p);
		break;
	case IL2CPP_TYPE_R4:
		*(float *)value = ReadFloat(p);
		break;
	case IL2CPP_TYPE_R8:
		*(double *)value = ReadDouble(p);
		break;
	case IL2CPP_TYPE_STRING:
	{
		*(void**)value = NULL;
		if (p != NULL)
		{
			// int32_t length followed by non-null terminated utf-8 byte stream
			uint32_t length = Read32(p);
			*(Il2CppString**)value = il2cpp::vm::String::NewLen(p + sizeof(uint32_t), length);
		}
		break;
	}
	case IL2CPP_TYPE_CLASS:
	case IL2CPP_TYPE_OBJECT:
	case IL2CPP_TYPE_GENERICINST:
	case IL2CPP_TYPE_SZARRAY:
		assert(p == NULL);
		*(void**)value = NULL;
		break;
	default:
		retval = -1;
		assert(0);
	}
	return retval;
}

} /* utils */
} /* il2cpp */
