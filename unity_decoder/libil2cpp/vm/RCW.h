#pragma once

#include "il2cpp-config.h"

struct Il2CppComObject;
struct Il2CppGuid;
struct Il2CppIUnknown;

namespace il2cpp
{
namespace vm
{

class LIBIL2CPP_CODEGEN_API RCW
{
public:
	static void Initialize(Il2CppComObject* rcw, const Il2CppGuid& clsid);
	static Il2CppIUnknown* QueryInterface(Il2CppComObject* rcw, const Il2CppGuid& iid, bool throwOnError);
	static Il2CppObject* Create(Il2CppIUnknown* unknown);
	static void Cleanup(Il2CppComObject* rcw);
};

} /* namespace vm */
} /* namespace il2cpp */
