#include "il2cpp-config.h"
#include "metadata/GenericMetadata.h"
#include "vm/Class.h"
#include "vm/ComObjectBase.h"
#include "vm/GenericClass.h"
#include "vm/MetadataCache.h"

using namespace il2cpp::vm;

il2cpp_hresult_t STDCALL ComObjectBase::GetRuntimeClassName(Il2CppHString* className)
{
    Il2CppClass* objectClass = GetManagedObjectInline()->klass;
    if (il2cpp_defaults.ireference_class != NULL && !Class::IsInflated(objectClass) && (Class::IsValuetype(objectClass) || objectClass == il2cpp_defaults.string_class))
    {
        // For value types/strings we're supposed to return the name of its boxed representation, i.e. Windows.Foundation.IReference`1<T>
        const Il2CppGenericInst* inst = MetadataCache::GetGenericInst(&objectClass->byval_arg, 1);
        Il2CppGenericClass* genericClass = metadata::GenericMetadata::GetGenericClass(il2cpp_defaults.ireference_class, inst);
        objectClass = GenericClass::GetClass(genericClass);
    }

    const char* name = MetadataCache::GetWindowsRuntimeClassName(objectClass);
    if (name == NULL)
    {
        *className = NULL;
        return IL2CPP_S_OK;
    }

    UTF16String nameUtf16 = utils::StringUtils::Utf8ToUtf16(name);
    return os::WindowsRuntime::CreateHString(utils::StringView<Il2CppChar>(nameUtf16.c_str(), nameUtf16.length()), className);
}

il2cpp_hresult_t STDCALL ComObjectBase::GetTrustLevel(int32_t* trustLevel)
{
    *trustLevel = 0;
    return IL2CPP_S_OK;
}

Il2CppObject* STDCALL ComObjectBase::GetManagedObject()
{
    return GetManagedObjectInline();
}

il2cpp_hresult_t STDCALL ComObjectBase::GetUnmarshalClass(const Il2CppGuid& iid, void* object, uint32_t context, void* reserved, uint32_t flags, Il2CppGuid* clsid)
{
    Il2CppIMarshal* freeThreadedMarshaler;
    il2cpp_hresult_t hr = GetFreeThreadedMarshalerNoAddRef(&freeThreadedMarshaler);
    if (IL2CPP_HR_FAILED(hr))
        return hr;

    return freeThreadedMarshaler->GetUnmarshalClass(iid, object, context, reserved, flags, clsid);
}

il2cpp_hresult_t STDCALL ComObjectBase::GetMarshalSizeMax(const Il2CppGuid& iid, void* object, uint32_t context, void* reserved, uint32_t flags, uint32_t* size)
{
    Il2CppIMarshal* freeThreadedMarshaler;
    il2cpp_hresult_t hr = GetFreeThreadedMarshalerNoAddRef(&freeThreadedMarshaler);
    if (IL2CPP_HR_FAILED(hr))
        return hr;

    return freeThreadedMarshaler->GetMarshalSizeMax(iid, object, context, reserved, flags, size);
}

il2cpp_hresult_t STDCALL ComObjectBase::MarshalInterface(Il2CppIStream* stream, const Il2CppGuid& iid, void* object, uint32_t context, void* reserved, uint32_t flags)
{
    Il2CppIMarshal* freeThreadedMarshaler;
    il2cpp_hresult_t hr = GetFreeThreadedMarshalerNoAddRef(&freeThreadedMarshaler);
    if (IL2CPP_HR_FAILED(hr))
        return hr;

    return freeThreadedMarshaler->MarshalInterface(stream, iid, object, context, reserved, flags);
}

il2cpp_hresult_t STDCALL ComObjectBase::UnmarshalInterface(Il2CppIStream* stream, const Il2CppGuid& iid, void** object)
{
    Il2CppIMarshal* freeThreadedMarshaler;
    il2cpp_hresult_t hr = GetFreeThreadedMarshalerNoAddRef(&freeThreadedMarshaler);
    if (IL2CPP_HR_FAILED(hr))
        return hr;

    return freeThreadedMarshaler->UnmarshalInterface(stream, iid, object);
}

il2cpp_hresult_t STDCALL ComObjectBase::ReleaseMarshalData(Il2CppIStream* stream)
{
    Il2CppIMarshal* freeThreadedMarshaler;
    il2cpp_hresult_t hr = GetFreeThreadedMarshalerNoAddRef(&freeThreadedMarshaler);
    if (IL2CPP_HR_FAILED(hr))
        return hr;

    return freeThreadedMarshaler->ReleaseMarshalData(stream);
}

il2cpp_hresult_t STDCALL ComObjectBase::DisconnectObject(uint32_t reserved)
{
    Il2CppIMarshal* freeThreadedMarshaler;
    il2cpp_hresult_t hr = GetFreeThreadedMarshalerNoAddRef(&freeThreadedMarshaler);
    if (IL2CPP_HR_FAILED(hr))
        return hr;

    return freeThreadedMarshaler->DisconnectObject(reserved);
}

il2cpp_hresult_t ComObjectBase::GetFreeThreadedMarshalerNoAddRef(Il2CppIMarshal** destination)
{
    Il2CppIMarshal* freeThreadedMarshaler = m_FreeThreadedMarshaler;
    if (freeThreadedMarshaler == NULL)
    {
        // We don't really want to aggregate FTM, as then we'd have to store its IUnknown too
        // So we pass NULL as the first parameter
        Il2CppIUnknown* freeThreadedMarshalerUnknown;
        il2cpp_hresult_t hr = COM::CreateFreeThreadedMarshaler(NULL, &freeThreadedMarshalerUnknown);
        if (IL2CPP_HR_FAILED(hr))
            return hr;

        hr = freeThreadedMarshalerUnknown->QueryInterface(Il2CppIMarshal::IID, reinterpret_cast<void**>(&freeThreadedMarshaler));
        freeThreadedMarshalerUnknown->Release();
        if (IL2CPP_HR_FAILED(hr))
            return hr;

        if (Atomic::CompareExchangePointer<Il2CppIMarshal>(&m_FreeThreadedMarshaler, freeThreadedMarshaler, NULL) != NULL)
        {
            freeThreadedMarshaler->Release();
            freeThreadedMarshaler = m_FreeThreadedMarshaler;
        }
    }

    *destination = freeThreadedMarshaler;
    return IL2CPP_S_OK;
}
