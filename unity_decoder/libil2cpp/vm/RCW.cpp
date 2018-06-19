#include "il2cpp-config.h"
#include "object-internals.h"
#include "class-internals.h"
#include "gc/GarbageCollector.h"
#include "vm/Exception.h"
#include "vm/Object.h"
#include "vm/PlatformInvoke.h"
#include "vm/RCW.h"
#include "os/COM.h"
#include "os/Mutex.h"
#include "utils/StdUnorderedMap.h"

const Il2CppGuid Il2CppIUnknown::IID = { 0x00000000, 0x0000, 0x0000, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 };
const Il2CppGuid Il2CppISequentialStream::IID = { 0x0c733a30, 0x2a1c, 0x11ce, 0xad, 0xe5, 0x00, 0xaa, 0x00, 0x44, 0x77, 0x3d };
const Il2CppGuid Il2CppIStream::IID = { 0x0000000c, 0x0000, 0x0000, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 };
const Il2CppGuid Il2CppIMarshal::IID = { 0x00000003, 0x0000, 0x0000, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 };
const Il2CppGuid Il2CppIManagedObject::IID = { 0xc3fcc19e, 0xa970, 0x11d2, 0x8b, 0x5a, 0x00, 0xa0, 0xc9, 0xb7, 0xc9, 0xc4 };
const Il2CppGuid Il2CppIInspectable::IID = { 0xaF86e2e0, 0xb12d, 0x4c6a, 0x9c, 0x5a, 0xd7, 0xaa, 0x65, 0x10, 0x1E, 0x90 };
const Il2CppGuid Il2CppIActivationFactory::IID = { 0x00000035, 0x0000, 0x0000, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 };

namespace il2cpp
{

using namespace os;

namespace vm
{

typedef unordered_map<Il2CppIUnknown*, Il2CppComObject*> RCWCache;

static FastMutex g_CacheMutex;
static RCWCache g_Cache;

void RCW::Initialize(Il2CppComObject* rcw, const Il2CppGuid& clsid)
{
	const il2cpp_hresult_t hr = COM::CreateInstance(clsid, &rcw->identity);
	Exception::RaiseIfFailed(hr);

	{
		FastAutoLock lock(&g_CacheMutex);
		const bool inserted = g_Cache.insert(std::make_pair(rcw->identity, rcw)).second;
		Assert(inserted);
	}
}

void RCW::Cleanup(Il2CppComObject* rcw)
{
	FastAutoLock lock(&g_CacheMutex);
	const size_t erased = g_Cache.erase(rcw->identity);
	Assert(1 == erased);
}

Il2CppIUnknown* RCW::QueryInterface(Il2CppComObject* rcw, const Il2CppGuid& iid, bool throwOnError)
{
	assert(rcw);
	assert(rcw->identity);
	Il2CppIUnknown* result;
	const il2cpp_hresult_t hr = rcw->identity->QueryInterface(iid, reinterpret_cast<void**>(&result));
	if (IL2CPP_HR_FAILED(hr))
	{
		if (throwOnError)
			Exception::Raise(hr);
		return NULL;
	}
	assert(result);
	return result;
}

Il2CppObject* RCW::Create(Il2CppIUnknown* unknown)
{
	if (!unknown)
		return NULL;

	il2cpp_hresult_t hr;

	// check for managed object. there's no need to create rcw for it

	Il2CppIManagedObject* managed;
	hr = unknown->QueryInterface(Il2CppIManagedObject::IID, reinterpret_cast<void**>(&managed));
	if (IL2CPP_HR_SUCCEEDED(hr))
	{
		assert(managed);

		Il2CppChar* bstr_guid;
		int32_t app_domain_id;
		intptr_t ccw;
		hr = managed->GetObjectIdentity(&bstr_guid, &app_domain_id, &ccw);
		managed->Release();
		Exception::RaiseIfFailed(hr);

		assert(!bstr_guid);
		assert(!app_domain_id);
		assert(ccw);

		PlatformInvoke::MarshalFreeBString(bstr_guid);

		return reinterpret_cast<Il2CppObject*>(ccw);
	}

	// identity is used to uniquely identify object

	Il2CppIUnknown* identity;
	hr = unknown->QueryInterface(Il2CppIUnknown::IID, reinterpret_cast<void**>(&identity));
	Exception::RaiseIfFailed(hr);
	assert(identity);

	Il2CppComObject* rcw;

	{
		FastAutoLock lock(&g_CacheMutex);

		// check if rcw has already been created to prevent duplication

		RCWCache::iterator iter = g_Cache.find(identity);
		if (iter != g_Cache.end())
		{
			identity->Release();
			return iter->second;
		}

		// create new rcw object otherwise

		rcw = static_cast<Il2CppComObject*>(Object::New(il2cpp_defaults.il2cpp_com_object_class));
		rcw->identity = identity;

		const bool inserted = g_Cache.insert(std::make_pair(rcw->identity, rcw)).second;
		Assert(inserted);
	}

	return rcw;
}

} /* namespace vm */
} /* namespace il2cpp */
