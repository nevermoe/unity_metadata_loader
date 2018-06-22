#if NET_4_0

#include "il2cpp-config.h"
#include "SafeStringMarshal.h"
#include "utils/StringUtils.h"

namespace il2cpp
{
namespace icalls
{
namespace mscorlib
{
namespace Mono
{
    Il2CppIntPtr SafeStringMarshal::StringToUtf8(Il2CppString* str)
    {
        Il2CppIntPtr res;

        std::string strobj = il2cpp::utils::StringUtils::Utf16ToUtf8(str->chars, str->length);
        size_t size = strobj.size();
        char *cstr = new char[size + 1];
        memcpy(cstr, strobj.c_str(), size);
        cstr[size] = 0;
        res.m_value = cstr;

        return res;
    }

    void SafeStringMarshal::GFree(Il2CppIntPtr ptr)
    {
        char *cstr = (char*)ptr.m_value;
        delete cstr;
    }
} // namespace Mono
} // namespace mscorlib
} // namespace icalls
} // namespace il2cpp

#endif
