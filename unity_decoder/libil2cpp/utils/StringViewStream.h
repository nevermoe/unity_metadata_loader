#pragma once

#include <ostream>
#include "StringView.h"

// This function is in a separate header file because we include StringView into generated code
// and we definitely don't want to include ostream into generated code

template<typename StreamCharType, typename CharType, typename Traits> inline
std::basic_ostream<StreamCharType, Traits>& operator<<(std::basic_ostream<StreamCharType, Traits>& ostream, const il2cpp::utils::StringView<CharType>& stringView)
{
    StringViewAsNullTerminatedStringOf(CharType, stringView, str);
    ostream << str;
    return ostream;
}
