#pragma once

#include <stdint.h>
#include <vector>
#include <string>
#include "il2cpp-config.h"
struct Il2CppAssemblyName;
struct Il2CppReflectionAssemblyName;

namespace il2cpp
{
namespace vm
{

class LIBIL2CPP_CODEGEN_API AssemblyName
{
// exported
public:
	static std::string AssemblyNameToString (const Il2CppAssemblyName& aname);
	static bool ParseName (Il2CppReflectionAssemblyName* aname, std::string assemblyName);
public:

private:
};

} /* namespace vm */
} /* namespace il2cpp */
