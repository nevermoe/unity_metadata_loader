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
namespace mscorlib
{
namespace Mono
{
namespace Globalization
{
namespace Unicode
{

class LIBIL2CPP_CODEGEN_API Normalization
{
public:
	static void load_normalization_resource (Il2CppIntPtr* argProps, Il2CppIntPtr* argMappedChars, Il2CppIntPtr* argCharMapIndex, Il2CppIntPtr* argHelperIndex, Il2CppIntPtr* argMapIdxToComposite, Il2CppIntPtr* argCombiningClass);
};

} /* namespace Unicode */
} /* namespace Globalization */
} /* namespace Mono */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
