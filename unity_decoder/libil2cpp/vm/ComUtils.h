#pragma once

#include "gc/GCHandle.h"
#include "utils/Memory.h"
#include "vm/Atomic.h"
#include "vm/COM.h"
#include "vm/Exception.h"

struct Il2CppObject;
struct Il2CppIUnknown;

namespace il2cpp
{
namespace vm
{

namespace details
{

struct ComNil;

template <typename I0 = ComNil, typename I1 = ComNil, typename I2 = ComNil, typename I3 = ComNil, typename I4 = ComNil, typename I5 = ComNil, typename I6 = ComNil, typename I7 = ComNil, typename I8 = ComNil, typename I9 = ComNil>
struct ComChain
{
	typedef I0 Current;
	typedef ComChain<I1, I2, I3, I4, I5, I6, I7, I8, I9, ComNil> Next;
};

template <>
struct ComChain<ComNil, ComNil, ComNil, ComNil, ComNil, ComNil, ComNil, ComNil, ComNil, ComNil>
{
	typedef Il2CppIUnknown Current;
};

typedef ComChain<> ComChainLast;

template <typename Chain>
struct ComInterface : Chain::Current, ComInterface<typename Chain::Next>
{
};

template <>
struct ComInterface<ComChainLast> : ComChainLast::Current
{
};

template <typename T, typename Chain>
struct QueryInterfaceHelper
{
	inline static il2cpp_hresult_t QueryInterface(T* self, const Il2CppGuid& iid, void** object)
	{
		if (!::memcmp(&iid, &Chain::Current::IID, sizeof(Il2CppGuid)))
		{
			*object = static_cast<typename Chain::Current*>(self);
			self->AddRef();
			return IL2CPP_S_OK;
		}
		return QueryInterfaceHelper<T, typename Chain::Next>::QueryInterface(self, iid, object);
	}
};

template <typename T>
struct QueryInterfaceHelper<T, ComChainLast>
{
	inline static il2cpp_hresult_t QueryInterface(T* self, const Il2CppGuid& iid, void** object)
	{
		NO_UNUSED_WARNING(self);
		NO_UNUSED_WARNING(iid);
		*object = NULL;
		return IL2CPP_E_NOINTERFACE;
	}
};

} /* namespace details */

template <typename T>
struct ComObject : T
{
public:
	inline static ComObject* __CreateInstance(Il2CppObject* obj)
	{
		void* memory = utils::Memory::Malloc(sizeof(ComObject));
		if (!memory)
			Exception::Raise(IL2CPP_E_OUTOFMEMORY);
		ComObject* instance = new(memory) ComObject(obj);
		instance->__Construct();
		return instance;
	}

	inline void __DestroyInstance()
	{
		this->~ComObject();
		utils::Memory::Free(this);
	}

private:
	inline ComObject(Il2CppObject* obj) : T(obj) {}
	ComObject(const ComObject&);
	ComObject& operator=(const ComObject&);
};

template <typename T, typename I0 = details::ComNil, typename I1 = details::ComNil, typename I2 = details::ComNil, typename I3 = details::ComNil, typename I4 = details::ComNil, typename I5 = details::ComNil, typename I6 = details::ComNil, typename I7 = details::ComNil, typename I8 = details::ComNil, typename I9 = details::ComNil>
struct NOVTABLE ComObjectBase : details::ComInterface<details::ComChain<I0, I1, I2, I3, I4, I5, I6, I7, I8, I9> >
{
private:
	volatile uint32_t __ref_count;

protected:
	uint32_t __handle;

public:
	inline ComObjectBase(Il2CppObject* obj) : __ref_count(0)
	{
		__handle = gc::GCHandle::New(obj, false);
	}

	inline ~ComObjectBase()
	{
		gc::GCHandle::Free(__handle);
	}

	virtual il2cpp_hresult_t STDCALL QueryInterface(const Il2CppGuid& iid, void** object)
	{
		if (!object)
			return IL2CPP_E_POINTER;
		if (!::memcmp(&iid, &Il2CppIUnknown::IID, sizeof(Il2CppGuid)))
		{
			*object = static_cast<typename details::ComChain<I0>::Current*>(this);
			AddRef();
			return IL2CPP_S_OK;
		}
		return details::QueryInterfaceHelper<ComObjectBase, details::ComChain<I0, I1, I2, I3, I4, I5, I6, I7, I8, I9> >::QueryInterface(this, iid, object);
	}

	virtual uint32_t STDCALL AddRef()
	{
		return Atomic::Increment(&__ref_count);
	}

	virtual uint32_t STDCALL Release()
	{
		const uint32_t count = Atomic::Decrement(&__ref_count);
		if (!count)
			static_cast<ComObject<T>*>(this)->__DestroyInstance();
		return count;
	}

protected:
	inline void __Construct() {}
};

template <typename T, typename I0 = details::ComNil, typename I1 = details::ComNil, typename I2 = details::ComNil, typename I3 = details::ComNil, typename I4 = details::ComNil, typename I5 = details::ComNil, typename I6 = details::ComNil, typename I7 = details::ComNil, typename I8 = details::ComNil>
struct NOVTABLE ManagedObjectBase : ComObjectBase<T, Il2CppIMarshal, Il2CppIManagedObject, I0, I1, I2, I3, I4, I5, I6, I7>
{
private:
	Il2CppIMarshal* __marshal;

public:
	inline ManagedObjectBase(Il2CppObject* obj) : ComObjectBase<T, Il2CppIMarshal, Il2CppIManagedObject, I0, I1, I2, I3, I4, I5, I6, I7>(obj), __marshal(NULL) {}

	inline ~ManagedObjectBase()
	{
		if (__marshal)
			__marshal->Release();
	}

	virtual il2cpp_hresult_t STDCALL GetUnmarshalClass(const Il2CppGuid& iid, void* object, uint32_t context, void* reserved, uint32_t flags, Il2CppGuid* clsid)
	{
		if (!__marshal)
			return IL2CPP_E_OUTOFMEMORY;
		return __marshal->GetUnmarshalClass(iid, object, context, reserved, flags, clsid);
	}

	virtual il2cpp_hresult_t STDCALL GetMarshalSizeMax(const Il2CppGuid& iid, void* object, uint32_t context, void* reserved, uint32_t flags, uint32_t* size)
	{
		if (!__marshal)
			return IL2CPP_E_OUTOFMEMORY;
		return __marshal->GetMarshalSizeMax(iid, object, context, reserved, flags, size);
	}

	virtual il2cpp_hresult_t STDCALL MarshalInterface(Il2CppIStream* stream, const Il2CppGuid& iid, void* object, uint32_t context, void* reserved, uint32_t flags)
	{
		if (!__marshal)
			return IL2CPP_E_OUTOFMEMORY;
		return __marshal->MarshalInterface(stream, iid, object, context, reserved, flags);
	}

	virtual il2cpp_hresult_t STDCALL UnmarshalInterface(Il2CppIStream* stream, const Il2CppGuid& iid, void** object)
	{
		if (!__marshal)
			return IL2CPP_E_OUTOFMEMORY;
		return __marshal->UnmarshalInterface(stream, iid, object);
	}

	virtual il2cpp_hresult_t STDCALL ReleaseMarshalData(Il2CppIStream* stream)
	{
		if (!__marshal)
			return IL2CPP_E_OUTOFMEMORY;
		return __marshal->ReleaseMarshalData(stream);
	}

	virtual il2cpp_hresult_t STDCALL DisconnectObject(uint32_t reserved)
	{
		if (!__marshal)
			return IL2CPP_E_OUTOFMEMORY;
		return __marshal->DisconnectObject(reserved);
	}

	virtual il2cpp_hresult_t STDCALL GetSerializedBuffer(Il2CppChar** bstr)
	{
		if (!bstr)
			return IL2CPP_E_POINTER;
		*bstr = NULL;
		return IL2CPP_E_NOTIMPL;
	}

	virtual il2cpp_hresult_t STDCALL GetObjectIdentity(Il2CppChar** bstr_guid, int32_t* app_domain_id, intptr_t* ccw)
	{
		if (!bstr_guid || !app_domain_id || !ccw)
			return IL2CPP_E_POINTER;
		*bstr_guid = NULL;
		*app_domain_id = 0;
		*ccw = reinterpret_cast<intptr_t>(gc::GCHandle::GetTarget(this->__handle));
		return IL2CPP_S_OK;
	}

protected:
	inline void __Construct()
	{
		Il2CppIUnknown* unknown;
		if (IL2CPP_HR_SUCCEEDED(COM::CreateFreeThreadedMarshaler(NULL, &unknown)))
		{
			unknown->QueryInterface(Il2CppIMarshal::IID, reinterpret_cast<void**>(&__marshal));
			unknown->Release();
		}
	}
};

} /* namespace vm */
} /* namespace il2cpp */
