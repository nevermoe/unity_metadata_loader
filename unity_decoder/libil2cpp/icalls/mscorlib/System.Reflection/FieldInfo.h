#pragma once

#include "object-internals.h"
#include "il2cpp-config.h"

struct Il2CppReflectionField;
struct Il2CppReflectionMarshal;

namespace il2cpp
{
namespace icalls
{
namespace mscorlib
{
namespace System
{
namespace Reflection
{

class LIBIL2CPP_CODEGEN_API FieldInfo
{
public:
	static Il2CppReflectionMarshal* GetUnmanagedMarshal (Il2CppReflectionField* field);
	static Il2CppReflectionField* internal_from_handle_type (Il2CppIntPtr field_handle, Il2CppIntPtr type_handle);
	static Il2CppArray* GetTypeModifiers (Il2CppReflectionField* field, bool optional);
};
	
} /* namespace Reflection */
} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
