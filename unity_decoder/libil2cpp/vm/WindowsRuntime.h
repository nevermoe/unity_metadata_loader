#pragma once
#include "utils/StringView.h"

struct Il2CppIActivationFactory;

namespace il2cpp
{
namespace vm
{

class LIBIL2CPP_CODEGEN_API WindowsRuntime
{
public:
	static Il2CppIActivationFactory* GetActivationFactory(const utils::StringView<Il2CppNativeChar>& runtimeClassName);
};

}
}