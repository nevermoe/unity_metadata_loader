#include "il2cpp-config.h"
#include "object-internals.h"
#include "class-internals.h"
#include "vm/Object.h"
#include "vm/CCW.h"
#include "vm/Class.h"
#include "vm/ComUtils.h"
#include "vm/Exception.h"
#include "vm/MetadataCache.h"

namespace il2cpp
{
namespace vm
{

namespace
{

struct NOVTABLE ManagedObject : ManagedObjectBase<ManagedObject>
{
	inline ManagedObject(Il2CppObject* obj) : ManagedObjectBase<ManagedObject>(obj) {}
};

} /* namespace anonymous */

typedef Il2CppIUnknown* (*CreateCCWFunc)(Il2CppObject* obj, const Il2CppGuid& iid);

Il2CppIUnknown* CCW::Create(Il2CppObject* obj, const Il2CppGuid& iid)
{
	if (!obj)
		return NULL;

	il2cpp_hresult_t hr;
	Il2CppIUnknown* result;

	// check for rcw object. com interface can be extracted from it and there's no need to create ccw
	if (obj->klass->is_import_or_windows_runtime)
	{
		hr = static_cast<Il2CppComObject*>(obj)->identity->QueryInterface(iid, reinterpret_cast<void**>(&result));
		Exception::RaiseIfFailed(hr);
		return result;
	}

	// check for ccw create function (implemented by com import types)

	const int32_t index = obj->klass->typeDefinition->ccwFunctionIndex;
	if (index != kMethodIndexInvalid)
	{
		const CreateCCWFunc createCcw = reinterpret_cast<CreateCCWFunc>(MetadataCache::GetCreateCcwFuncFromIndex(index));
		assert(createCcw);
		return createCcw(obj, iid);
	}

	// otherwise create generic ccw object that only implements IUnknown, IMarshal and IManagedObject interfaces

	ComObject<ManagedObject>* instance = ComObject<ManagedObject>::__CreateInstance(obj);
	hr = instance->QueryInterface(iid, reinterpret_cast<void**>(&result));
	if (IL2CPP_HR_FAILED(hr))
	{
		instance->__DestroyInstance();
		Exception::Raise(hr);
	}
	return result;
}

} /* namespace vm */
} /* namespace il2cpp */
