#pragma once

#include "il2cpp-config.h"
#include "blob.h"
#include "metadata.h"
#include "object-internals.h"
#include "vm/Array.h"
#include "vm/Class.h"
#include "vm/MarshalAlloc.h"
#include "vm/Object.h"
#include "vm/String.h"
#include "utils/StringView.h"

struct Il2CppString;
struct Il2CppStringBuilder;

struct PInvokeArguments
{
	const il2cpp::utils::StringView<Il2CppNativeChar> moduleName;
	const il2cpp::utils::StringView<char> entryPoint;
	Il2CppCallConvention callingConvention;
	Il2CppCharSet charSet;
	int parameterSize;
	bool isNoMangle;	// Says whether P/Invoke should append to function name 'A'/'W' according to charSet.
};

namespace il2cpp
{
namespace vm
{

class LIBIL2CPP_CODEGEN_API PlatformInvoke
{
public:
	static void SetFindPluginCallback(Il2CppSetFindPlugInCallback method);
	static Il2CppMethodPointer Resolve(const PInvokeArguments& pinvokeArgs);

	static void MarshalFree(void* ptr);

	static char* MarshalCSharpStringToCppString(Il2CppString* managedString);
	static void MarshalCSharpStringToCppStringFixed(Il2CppString* managedString, char* buffer, int numberOfCharacters);
	static Il2CppChar* MarshalCSharpStringToCppWString(Il2CppString* managedString);
	static void MarshalCSharpStringToCppWStringFixed(Il2CppString* managedString, Il2CppChar* buffer, int numberOfCharacters);
	static il2cpp_hresult_t MarshalCSharpStringToCppBStringNoThrow(Il2CppString* managedString, Il2CppChar** bstr);
	static Il2CppChar* MarshalCSharpStringToCppBString(Il2CppString* managedString);

	static Il2CppString* MarshalCppStringToCSharpStringResult(const char* value);
	static Il2CppString* MarshalCppWStringToCSharpStringResult(const Il2CppChar* value);
	static Il2CppString* MarshalCppBStringToCSharpStringResult(const Il2CppChar* value);

	static void MarshalFreeBString(Il2CppChar* value);

	static char** MarshalAllocateNativeStringArray(size_t size);
	static Il2CppChar** MarshalAllocateNativeWStringArray(size_t size);
	static Il2CppChar** MarshalAllocateNativeBStringArray(size_t size);

	static void MarshalStringArrayOut(char** nativeArray, Il2CppArray* managedArray);
	static void MarshalWStringArrayOut(Il2CppChar** nativeArray, Il2CppArray* managedArray);
	static void MarshalBStringArrayOut(Il2CppChar** nativeArray, Il2CppArray* managedArray);

	static void MarshalStringArray(Il2CppArray* managedArray, char** nativeArray);
	static void MarshalWStringArray(Il2CppArray* managedArray, Il2CppChar** nativeArray);
	static void MarshalBStringArray(Il2CppArray* managedArray, Il2CppChar** nativeArray);

	static Il2CppArray* MarshalStringArrayResult(char** nativeArray, size_t size);
	static Il2CppArray* MarshalWStringArrayResult(Il2CppChar** nativeArray, size_t size);
	static Il2CppArray* MarshalBStringArrayResult(Il2CppChar** nativeArray, size_t size);

	static char* MarshalStringBuilder(Il2CppStringBuilder* stringBuilder);
	static Il2CppChar* MarshalWStringBuilder(Il2CppStringBuilder* stringBuilder);

	static void MarshalStringBuilderResult(Il2CppStringBuilder* stringBuilder, char* buffer);
	static void MarshalWStringBuilderResult(Il2CppStringBuilder* stringBuilder, Il2CppChar* buffer);

	static void MarshalFreeStringArray(void** nativeArray, size_t size);
	static void MarshalFreeBStringArray(Il2CppChar** nativeArray, size_t size);

	static Il2CppIntPtr MarshalDelegate(Il2CppDelegate* d);
	static Il2CppDelegate* MarshalFunctionPointerToDelegate(void* functionPtr, Il2CppClass* delegateType);

	static void MarshalStructToNative(void* managedStructure, void* marshaledStructure, Il2CppClass* type);
	static void MarshalStructFromNative(void* marshaledStructure, void* managedStructure, Il2CppClass* type);
	static bool MarshalFreeStruct(void* marshaledStructure, Il2CppClass* type);

	template <typename ElementType>
	static ElementType* MarshalArray(Il2CppArray* managedArray)
	{
		if (managedArray == NULL)
			return NULL;

		return (ElementType*)Array::GetFirstElementAddress(managedArray);
	}

	template <typename ElementType>
	static Il2CppArray* MarshalArrayResult(Il2CppClass* type, ElementType* nativeArray, size_t size)
	{
		if (nativeArray == NULL)
			return NULL;

		Il2CppArray* managedArray = Array::New(type, (il2cpp_array_size_t)size);
		MarshalArrayOut(nativeArray, managedArray);

		return managedArray;
	}

	template <typename ElementType>
	static void MarshalArrayOut(ElementType* nativeArray, Il2CppArray* managedArray)
	{
		for (il2cpp_array_size_t i = 0; i < managedArray->max_length; ++i)
			il2cpp_array_setwithsize(managedArray, sizeof(ElementType), i, nativeArray[i]);
	}

	template <typename T>
	static T* MarshalAllocateStringBuffer(size_t numberOfCharacters)
	{
		return (T*)MarshalAlloc::Allocate(numberOfCharacters * sizeof(T));
	}
};

} /* namespace vm */
} /* namespace il2cpp */
