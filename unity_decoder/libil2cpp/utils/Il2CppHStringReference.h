#pragma once

#include <object-internals.h>
#include "StringView.h"
#include "os/WindowsRuntime.h"
#include "vm/Exception.h"

namespace il2cpp
{

namespace utils
{

class Il2CppHStringReference
{
private:
	Il2CppHString m_String;
	Il2CppHStringHeader m_Header;

public:
	inline Il2CppHStringReference(const StringView<Il2CppNativeChar>& str)
	{
		il2cpp_hresult_t hr = il2cpp::os::WindowsRuntime::CreateHStringReference(str, &m_Header, &m_String);
		il2cpp::vm::Exception::RaiseIfFailed(hr);
	}

	inline operator Il2CppHString() const
	{
		return m_String;
	}
};

}
}