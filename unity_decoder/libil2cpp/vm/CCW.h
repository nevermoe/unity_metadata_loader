#pragma once

struct Il2CppIUnknown;
struct Il2CppObject;

namespace il2cpp
{
namespace vm
{

class LIBIL2CPP_CODEGEN_API CCW
{
public:
	static Il2CppIUnknown* Create(Il2CppObject* obj, const Il2CppGuid& iid);
};

} /* namespace vm */
} /* namespace il2cpp */
