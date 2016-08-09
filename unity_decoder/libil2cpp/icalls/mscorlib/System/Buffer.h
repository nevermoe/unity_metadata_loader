#pragma once

#include <stdint.h>
#include "il2cpp-config.h"
#include "object-internals.h"

struct Il2CppArray;

namespace il2cpp
{
namespace icalls
{
namespace mscorlib
{
namespace System
{

class LIBIL2CPP_CODEGEN_API Buffer
{
public:
	static bool BlockCopyInternal (Il2CppArray * src, int src_offset, Il2CppArray * dest, int dest_offset, int count);
	static int32_t ByteLengthInternal (Il2CppArray* arr);
	static uint8_t GetByteInternal (Il2CppArray* arr, int idx);
	static void SetByteInternal (Il2CppArray* arr, int idx, int value);
};

} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
