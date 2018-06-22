#pragma once

#include <object-internals.h>
#include "StringView.h"
#include "vm/Exception.h"
#include "vm/WindowsRuntime.h"

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
            il2cpp::vm::WindowsRuntime::CreateHStringReference(str, &m_Header, &m_String);
        }

        inline operator Il2CppHString() const
        {
            return m_String;
        }
    };
}
}
