#pragma once

#include <stdint.h>
#include "il2cpp-config.h"
#include "object-internals.h"

struct Il2CppReflectionType;
struct mscorlib_System___ComObject;

namespace il2cpp
{
namespace icalls
{
namespace mscorlib
{
namespace System
{

class LIBIL2CPP_CODEGEN_API __ComObject
{
public:
	static mscorlib_System___ComObject * CreateRCW (Il2CppReflectionType * t);
	static void ReleaseInterfaces (mscorlib_System___ComObject * __this);
	static Il2CppIntPtr GetInterfaceInternal (mscorlib_System___ComObject * __this, Il2CppReflectionType * t, bool throwException);
};

} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
