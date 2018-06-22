#pragma once

struct Il2CppGuid;
struct Il2CppSafeArrayBound;
struct Il2CppSafeArray;
struct Il2CppIUnknown;

namespace il2cpp
{
namespace vm
{

class LIBIL2CPP_CODEGEN_API COM
{
public:
	static il2cpp_hresult_t CreateInstance(const Il2CppGuid& clsid, Il2CppIUnknown** object);
	static il2cpp_hresult_t CreateFreeThreadedMarshaler(Il2CppIUnknown* outer, Il2CppIUnknown** marshal);
	static void MarshalVariant(Il2CppObject* obj, Il2CppVariant* variant);
	static Il2CppObject* MarshalVariantResult(Il2CppVariant* variant);
	static void DestroyVariant(Il2CppVariant* variant);
	static Il2CppSafeArray* MarshalSafeArray(uint16_t variantType, Il2CppArray* managedArray);
	static Il2CppArray* MarshalSafeArrayResult(uint16_t variantType, Il2CppClass* type, Il2CppSafeArray* safeArray);
	static Il2CppSafeArray* MarshalSafeArrayBString(Il2CppArray* managedArray);
	static Il2CppArray* MarshalSafeArrayBStringResult(Il2CppClass* type, Il2CppSafeArray* safeArray);
	static void DestroySafeArray(Il2CppSafeArray* safeArray);
};

} /* namespace vm */
} /* namespace il2cpp */
