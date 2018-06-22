#pragma once

#include <stdint.h>
#include "il2cpp-config.h"
#include "object-internals.h"

struct Il2CppObject;
struct Il2CppDelegate;
struct Il2CppReflectionType;
struct Il2CppReflectionMethod;
struct Il2CppReflectionField;
struct Il2CppArray;
struct Il2CppException;
struct Il2CppReflectionModule;
struct Il2CppAssembly;
struct Il2CppAssemblyName;
struct Il2CppAppDomain;

namespace il2cpp
{
namespace icalls
{
namespace System
{
namespace System
{
namespace Diagnostics
{

struct Il2CppCounterSample;

class LIBIL2CPP_CODEGEN_API PerformanceCounter
{
public:
	static void FreeData (Il2CppIntPtr impl);
	static bool GetSample (Il2CppIntPtr impl, bool only_value, Il2CppCounterSample* sample);
	static int64_t UpdateValue (Il2CppIntPtr impl, bool do_incr, int64_t value);
	static Il2CppIntPtr GetImpl (Il2CppString* category, Il2CppString* counter, Il2CppString* instance, Il2CppString* machine, int* type, bool* custom);
};

} /* namespace Diagnostics */
} /* namespace System */
} /* namespace System */
} /* namespace icalls */
} /* namespace il2cpp */
