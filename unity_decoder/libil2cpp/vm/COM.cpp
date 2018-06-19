#include "il2cpp-config.h"
#include "object-internals.h"
#include "vm/Array.h"
#include "vm/COM.h"
#include "vm/Exception.h"
#include "vm/PlatformInvoke.h"
#include "vm/Reflection.h"
#include "os/COM.h"

namespace il2cpp
{
namespace vm
{

il2cpp_hresult_t COM::CreateInstance(const Il2CppGuid& clsid, Il2CppIUnknown** object)
{
	return os::COM::CreateInstance(clsid, object);
}

il2cpp_hresult_t COM::CreateFreeThreadedMarshaler(Il2CppIUnknown* outer, Il2CppIUnknown** marshal)
{
	return os::COM::CreateFreeThreadedMarshaler(outer, marshal);
}

void COM::MarshalVariant(Il2CppObject* obj, Il2CppVariant* variant)
{
	assert(variant);
	os::COM::VariantInit(variant);

	if (!obj)
		return;

	if (obj->klass == il2cpp_defaults.sbyte_class)
	{
		variant->n1.n2.type = IL2CPP_VT_I1;
		variant->n1.n2.n3.cVal = *static_cast<int8_t*>(Object::Unbox(obj));
	}
	else if (obj->klass == il2cpp_defaults.byte_class)
	{
		variant->n1.n2.type = IL2CPP_VT_UI1;
		variant->n1.n2.n3.bVal = *static_cast<uint8_t*>(Object::Unbox(obj));
	}
	else if (obj->klass == il2cpp_defaults.int16_class)
	{
		variant->n1.n2.type = IL2CPP_VT_I2;
		variant->n1.n2.n3.iVal = *static_cast<int16_t*>(Object::Unbox(obj));
	}
	else if (obj->klass == il2cpp_defaults.uint16_class)
	{
		variant->n1.n2.type = IL2CPP_VT_UI2;
		variant->n1.n2.n3.uiVal = *static_cast<uint16_t*>(Object::Unbox(obj));
	}
	else if (obj->klass == il2cpp_defaults.int32_class)
	{
		variant->n1.n2.type = IL2CPP_VT_I4;
		variant->n1.n2.n3.lVal = *static_cast<int32_t*>(Object::Unbox(obj));
	}
	else if (obj->klass == il2cpp_defaults.uint32_class)
	{
		variant->n1.n2.type = IL2CPP_VT_UI4;
		variant->n1.n2.n3.ulVal = *static_cast<uint32_t*>(Object::Unbox(obj));
	}
	else if (obj->klass == il2cpp_defaults.int64_class)
	{
		variant->n1.n2.type = IL2CPP_VT_I8;
		variant->n1.n2.n3.llVal = *static_cast<int64_t*>(Object::Unbox(obj));
	}
	else if (obj->klass == il2cpp_defaults.uint64_class)
	{
		variant->n1.n2.type = IL2CPP_VT_UI8;
		variant->n1.n2.n3.ullVal = *static_cast<uint64_t*>(Object::Unbox(obj));
	}
	else if (obj->klass == il2cpp_defaults.single_class)
	{
		variant->n1.n2.type = IL2CPP_VT_R4;
		variant->n1.n2.n3.fltVal = *static_cast<float*>(Object::Unbox(obj));
	}
	else if (obj->klass == il2cpp_defaults.double_class)
	{
		variant->n1.n2.type = IL2CPP_VT_R8;
		variant->n1.n2.n3.dblVal = *static_cast<double*>(Object::Unbox(obj));
	}
	else if (obj->klass == il2cpp_defaults.boolean_class)
	{
		variant->n1.n2.type = IL2CPP_VT_BOOL;
		variant->n1.n2.n3.boolVal = *static_cast<bool*>(Object::Unbox(obj)) ? IL2CPP_VARIANT_TRUE : IL2CPP_VARIANT_FALSE;
	}
	else if (obj->klass == il2cpp_defaults.string_class)
	{
		variant->n1.n2.type = IL2CPP_VT_BSTR;
		variant->n1.n2.n3.bstrVal = PlatformInvoke::MarshalCSharpStringToCppBString(reinterpret_cast<Il2CppString*>(obj));
	}
	else if (obj->klass == il2cpp_defaults.dbnull_class)
	{
		variant->n1.n2.type = IL2CPP_VT_NULL;
	}
	else if (obj->klass == il2cpp_defaults.error_wrapper_class)
	{
		variant->n1.n2.type = IL2CPP_VT_ERROR;
		variant->n1.n2.n3.scode = reinterpret_cast<Il2CppErrorWrapper*>(obj)->errorCode;
	}
	else if (obj->klass == il2cpp_defaults.missing_class)
	{
		variant->n1.n2.type = IL2CPP_VT_ERROR;
		variant->n1.n2.n3.scode = IL2CPP_DISP_E_PARAMNOTFOUND;
	}
	else
	{
		Exception::Raise(IL2CPP_E_INVALIDARG);
	}
}

Il2CppObject* COM::MarshalVariantResult(Il2CppVariant* variant)
{
	assert(variant);
	switch (variant->n1.n2.type)
	{
	case IL2CPP_VT_EMPTY:
		return NULL;
	case IL2CPP_VT_NULL:
		return Reflection::GetDBNullObject();
	case IL2CPP_VT_ERROR:
		return Object::Box(il2cpp_defaults.int32_class, &variant->n1.n2.n3.scode);
	case IL2CPP_VT_I1:
		return Object::Box(il2cpp_defaults.sbyte_class, &variant->n1.n2.n3.cVal);
	case IL2CPP_VT_UI1:
		return Object::Box(il2cpp_defaults.byte_class, &variant->n1.n2.n3.bVal);
	case IL2CPP_VT_I2:
		return Object::Box(il2cpp_defaults.int16_class, &variant->n1.n2.n3.iVal);
	case IL2CPP_VT_UI2:
		return Object::Box(il2cpp_defaults.uint16_class, &variant->n1.n2.n3.uiVal);
	case IL2CPP_VT_I4:
		return Object::Box(il2cpp_defaults.int32_class, &variant->n1.n2.n3.lVal);
	case IL2CPP_VT_UI4:
		return Object::Box(il2cpp_defaults.uint32_class, &variant->n1.n2.n3.ulVal);
	case IL2CPP_VT_I8:
		return Object::Box(il2cpp_defaults.int64_class, &variant->n1.n2.n3.llVal);
	case IL2CPP_VT_UI8:
		return Object::Box(il2cpp_defaults.uint64_class, &variant->n1.n2.n3.ullVal);
	case IL2CPP_VT_R4:
		return Object::Box(il2cpp_defaults.single_class, &variant->n1.n2.n3.fltVal);
	case IL2CPP_VT_R8:
		return Object::Box(il2cpp_defaults.double_class, &variant->n1.n2.n3.dblVal);
	case IL2CPP_VT_BOOL:
	{
		assert(variant->n1.n2.n3.boolVal == IL2CPP_VARIANT_FALSE || variant->n1.n2.n3.boolVal == IL2CPP_VARIANT_TRUE);
		bool value = variant->n1.n2.n3.boolVal != IL2CPP_VARIANT_FALSE;
		return Object::Box(il2cpp_defaults.boolean_class, &value);
	}
	case IL2CPP_VT_BSTR:
		return reinterpret_cast<Il2CppObject*>(PlatformInvoke::MarshalCppBStringToCSharpStringResult(variant->n1.n2.n3.bstrVal));
	default:
		Exception::Raise(IL2CPP_E_INVALIDARG);
		return NULL;
	}
}

void COM::DestroyVariant(Il2CppVariant* variant)
{
	const il2cpp_hresult_t hr = os::COM::VariantClear(variant);
	Exception::RaiseIfFailed(hr);
}

Il2CppSafeArray* COM::MarshalSafeArray(uint16_t variantType, Il2CppArray* managedArray)
{
	if (!managedArray)
		return NULL;

	Il2CppSafeArrayBound bounds[1];
	bounds[0].element_count = Array::GetLength(managedArray);
	bounds[0].lower_bound = 0;
	Il2CppSafeArray* safeArray = os::COM::SafeArrayCreate(variantType, 1, bounds);
	if (!safeArray)
		Exception::Raise(IL2CPP_E_OUTOFMEMORY);

	void* data;
	il2cpp_hresult_t hr = os::COM::SafeArrayAccessData(safeArray, &data);
	if (IL2CPP_HR_FAILED(hr))
	{
		os::COM::SafeArrayDestroy(safeArray);
		Exception::Raise(hr);
	}

	::memcpy(data, Array::GetFirstElementAddress(managedArray), Array::GetByteLength(managedArray));

	hr = os::COM::SafeArrayUnaccessData(safeArray);
	if (IL2CPP_HR_FAILED(hr))
	{
		os::COM::SafeArrayDestroy(safeArray);
		Exception::Raise(hr);
	}

	return safeArray;
}

Il2CppArray* COM::MarshalSafeArrayResult(uint16_t variantType, Il2CppClass* type, Il2CppSafeArray* safeArray)
{
	if (!safeArray)
		return NULL;

	uint16_t actualVariantType;
	il2cpp_hresult_t hr = os::COM::SafeArrayGetVartype(safeArray, &actualVariantType);
	Exception::RaiseIfFailed(hr);
	if (actualVariantType != variantType)
		Exception::Raise(IL2CPP_E_INVALIDARG);

	const uint32_t actualDimentionCount = os::COM::SafeArrayGetDim(safeArray);
	if (actualDimentionCount != 1)
		Exception::Raise(IL2CPP_E_INVALIDARG);

	int32_t lowerBound;
	hr = os::COM::SafeArrayGetLBound(safeArray, 1, &lowerBound);
	Exception::RaiseIfFailed(hr);

	int32_t upperBound;
	hr = os::COM::SafeArrayGetUBound(safeArray, 1, &upperBound);
	Exception::RaiseIfFailed(hr);

	const il2cpp_array_size_t size = static_cast<il2cpp_array_size_t>(upperBound - lowerBound + 1);
	Il2CppArray* managedArray = Array::New(type, size);

	void* data;
	hr = os::COM::SafeArrayAccessData(safeArray, &data);
	Exception::RaiseIfFailed(hr);

	::memcpy(Array::GetFirstElementAddress(managedArray), data, Array::GetByteLength(managedArray));

	hr = os::COM::SafeArrayUnaccessData(safeArray);
	Exception::RaiseIfFailed(hr);

	return managedArray;
}

Il2CppSafeArray* COM::MarshalSafeArrayBString(Il2CppArray* managedArray)
{
	if (!managedArray)
		return NULL;

	const uint32_t size = Array::GetLength(managedArray);

	Il2CppSafeArrayBound bounds[1];
	bounds[0].element_count = size;
	bounds[0].lower_bound = 0;
	Il2CppSafeArray* safeArray = os::COM::SafeArrayCreate(IL2CPP_VT_BSTR, 1, bounds);
	if (!safeArray)
		Exception::Raise(IL2CPP_E_OUTOFMEMORY);

	Il2CppChar** data;
	il2cpp_hresult_t hr = os::COM::SafeArrayAccessData(safeArray, reinterpret_cast<void**>(&data));
	if (IL2CPP_HR_FAILED(hr))
	{
		os::COM::SafeArrayDestroy(safeArray);
		Exception::Raise(hr);
	}

	for (uint32_t i = 0; i < size; ++i)
	{
		Il2CppString* managedString = il2cpp_array_get(managedArray, Il2CppString*, i);
		hr = PlatformInvoke::MarshalCSharpStringToCppBStringNoThrow(managedString, data + i);
		if (IL2CPP_HR_FAILED(hr))
		{
			os::COM::SafeArrayUnaccessData(safeArray);
			os::COM::SafeArrayDestroy(safeArray);
			Exception::Raise(hr);
		}
	}

	hr = os::COM::SafeArrayUnaccessData(safeArray);
	if (IL2CPP_HR_FAILED(hr))
	{
		os::COM::SafeArrayDestroy(safeArray);
		Exception::Raise(hr);
	}

	return safeArray;
}

Il2CppArray* COM::MarshalSafeArrayBStringResult(Il2CppClass* type, Il2CppSafeArray* safeArray)
{
	if (!safeArray)
		return NULL;

	uint16_t actualVariantType;
	il2cpp_hresult_t hr = os::COM::SafeArrayGetVartype(safeArray, &actualVariantType);
	Exception::RaiseIfFailed(hr);
	if (actualVariantType != IL2CPP_VT_BSTR)
		Exception::Raise(IL2CPP_E_INVALIDARG);

	const uint32_t actualDimentionCount = os::COM::SafeArrayGetDim(safeArray);
	if (actualDimentionCount != 1)
		Exception::Raise(IL2CPP_E_INVALIDARG);

	int32_t lowerBound;
	hr = os::COM::SafeArrayGetLBound(safeArray, 1, &lowerBound);
	Exception::RaiseIfFailed(hr);

	int32_t upperBound;
	hr = os::COM::SafeArrayGetUBound(safeArray, 1, &upperBound);
	Exception::RaiseIfFailed(hr);

	const il2cpp_array_size_t size = static_cast<il2cpp_array_size_t>(upperBound - lowerBound + 1);
	Il2CppArray* managedArray = Array::New(il2cpp_defaults.string_class, size);

	Il2CppChar** data;
	hr = os::COM::SafeArrayAccessData(safeArray, reinterpret_cast<void**>(&data));
	Exception::RaiseIfFailed(hr);

	for (il2cpp_array_size_t i = 0; i < size; ++i)
		il2cpp_array_setref(managedArray, i, PlatformInvoke::MarshalCppBStringToCSharpStringResult(data[i]));

	hr = os::COM::SafeArrayUnaccessData(safeArray);
	Exception::RaiseIfFailed(hr);

	return managedArray;
}

void COM::DestroySafeArray(Il2CppSafeArray* safeArray)
{
	const il2cpp_hresult_t hr = os::COM::SafeArrayDestroy(safeArray);
	Exception::RaiseIfFailed(hr);
}

} /* namespace vm */
} /* namespace il2cpp */
