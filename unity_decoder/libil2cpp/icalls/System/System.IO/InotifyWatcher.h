#pragma once

#include <stdint.h>
#include "object-internals.h"
#include "il2cpp-config.h"

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
namespace IO
{

typedef int32_t InotifyMask;

class LIBIL2CPP_CODEGEN_API InotifyWatcher
{
public:
	static Il2CppIntPtr RemoveWatch (Il2CppIntPtr fd, int32_t wd);
	static int32_t AddWatch (Il2CppIntPtr fd, Il2CppString* name, InotifyMask mask);
	static Il2CppIntPtr GetInotifyInstance ();
};

} /* namespace IO */
} /* namespace System */
} /* namespace System */
} /* namespace icalls */
} /* namespace il2cpp */
