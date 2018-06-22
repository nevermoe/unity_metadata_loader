#pragma once

#include <stdint.h>
#include "il2cpp-config.h"
#include "object-internals.h"

struct Il2CppObject;
struct Il2CppArray;
struct Il2CppString;
struct Il2CppReflectionMethod;
struct Il2CppReflectionModuleBuilder;
struct Il2CppReflectionTypeBuilder;
struct Il2CppReflectionType;

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
namespace Emit
{

class LIBIL2CPP_CODEGEN_API ModuleBuilder
{
public:
	static void RegisterToken (Il2CppReflectionModuleBuilder*, Il2CppObject*, int);
	static void WriteToFile (Il2CppReflectionModuleBuilder*, Il2CppIntPtr);
	static void basic_init (Il2CppReflectionModuleBuilder*);
	static void build_metadata (Il2CppReflectionModuleBuilder*);
	static Il2CppReflectionType * create_modified_type (Il2CppReflectionTypeBuilder*, Il2CppString*);
	static int32_t getToken (Il2CppReflectionModuleBuilder*, Il2CppObject*);
	static int32_t getUSIndex (Il2CppReflectionModuleBuilder*, Il2CppString*);
	static void set_wrappers_type (Il2CppReflectionModuleBuilder*, Il2CppReflectionType*);
	static int32_t getMethodToken (Il2CppReflectionModuleBuilder*, Il2CppReflectionMethod*, Il2CppArray*);
};
	
} /* namespace Emit */
} /* namespace Reflection */
} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
