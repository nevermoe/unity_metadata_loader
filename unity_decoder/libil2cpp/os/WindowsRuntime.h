#pragma once

#include "object-internals.h"
#include "utils/StringView.h"

namespace il2cpp
{
namespace os
{

class LIBIL2CPP_CODEGEN_API WindowsRuntime
{
public:
	static il2cpp_hresult_t GetActivationFactory(Il2CppHString className, Il2CppIActivationFactory** activationFactory);

	static il2cpp_hresult_t CreateHStringReference(const utils::StringView<Il2CppNativeChar>& str, Il2CppHStringHeader* header, Il2CppHString* hstring);
};

}
}