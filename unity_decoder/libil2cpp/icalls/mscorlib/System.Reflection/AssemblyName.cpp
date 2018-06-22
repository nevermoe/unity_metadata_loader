#include "il2cpp-config.h"
#include "object-internals.h"
#include "mono-structs.h"

#include "icalls/mscorlib/System.Reflection/AssemblyName.h"
#include "vm/Array.h"
#include "vm/Class.h"
#include "vm/Exception.h"
#include "vm/Object.h"
#include "vm/Runtime.h"
#include "vm/String.h"
#include "vm/Type.h"
#include "vm/Reflection.h"
#include "vm/AssemblyName.h"
#include "utils/StringUtils.h"
#include "vm-utils/VmStringUtils.h"

using il2cpp::vm::Array;
using il2cpp::vm::Class;
using il2cpp::vm::Object;
using il2cpp::vm::Runtime;
using il2cpp::vm::String;

namespace il2cpp
{
namespace icalls
{
namespace mscorlib
{
namespace System
{
namespace Reflection
{
    bool AssemblyName::ParseName(Il2CppReflectionAssemblyName* aname, Il2CppString* assemblyName)
    {
        return vm::AssemblyName::ParseName(aname, utils::StringUtils::Utf16ToUtf8(utils::StringUtils::GetChars(assemblyName)));
    }

#if NET_4_0
    void AssemblyName::get_public_token(uint8_t* token, uint8_t* pubkey, int32_t len)
    {
        NOT_IMPLEMENTED_ICALL(AssemblyName::get_public_token);
        IL2CPP_UNREACHABLE;
    }

    char hexChar(uint8_t value)
    {
        if (value < 10)
            return '0' + value;
        else
            return 'A' + (value - 10);
    }

    Il2CppMonoAssemblyName* AssemblyName::GetNativeName(Il2CppIntPtr assembly_ptr)
    {
        Il2CppAssembly *assembly = (Il2CppAssembly*)assembly_ptr.m_value;

        Il2CppMonoAssemblyName *aname = (Il2CppMonoAssemblyName*)il2cpp::vm::Reflection::GetMonoAssemblyName(assembly);
        if (aname)
        {
            return aname;
        }
        else
        {
            aname = (Il2CppMonoAssemblyName*)IL2CPP_MALLOC_ZERO(sizeof(Il2CppMonoAssemblyName));
            aname->name.m_value = (void*)il2cpp::vm::MetadataCache::GetStringFromIndex(assembly->aname.nameIndex);
            aname->culture.m_value = (void*)il2cpp::vm::MetadataCache::GetStringFromIndex(assembly->aname.cultureIndex);
            aname->hash_value.m_value = (void*)il2cpp::vm::MetadataCache::GetStringFromIndex(assembly->aname.hashValueIndex);
            aname->public_key.m_value = (void*)il2cpp::vm::MetadataCache::GetStringFromIndex(assembly->aname.publicKeyIndex);
            aname->hash_alg = assembly->aname.hash_alg;
            aname->hash_len = assembly->aname.hash_len;
            aname->flags = assembly->aname.flags;
            aname->major = assembly->aname.major;
            aname->minor = assembly->aname.minor;
            aname->build = assembly->aname.build;
            aname->revision = assembly->aname.revision;

            //Mono public key token is stored as hexadecimal characters
            if (assembly->aname.publicKeyToken[0])
            {
                int j = 0;
                for (int i = 0; i < kPublicKeyByteLength; ++i)
                {
                    uint8_t value = assembly->aname.publicKeyToken[i];
                    aname->public_key_token.padding[j++] = hexChar((value & 0xF0) >> 4);
                    aname->public_key_token.padding[j++] = hexChar(value & 0x0F);
                }
            }

            il2cpp::vm::Reflection::SetMonoAssemblyName(assembly, aname);
            return aname;
        }
    }

    bool AssemblyName::ParseAssemblyName(Il2CppIntPtr namePtr, Il2CppMonoAssemblyName* aname, bool* is_version_defined, bool* is_token_defined)
    {
        std::string name((char*)namePtr.m_value);

        il2cpp::vm::TypeNameParseInfo info;
        il2cpp::vm::TypeNameParser parser(name, info, false);

        if (!parser.ParseAssembly())
            return false;

        if (is_version_defined)
        {
            *is_version_defined = false;
            size_t index = name.find("Version");
            if (index != std::string::npos)
                *is_version_defined = true;
        }

        if (is_token_defined)
        {
            *is_token_defined = false;
            size_t index = name.find("PublicKeyToken");
            if (index != std::string::npos)
                *is_token_defined = true;
        }

        const il2cpp::vm::TypeNameParseInfo::AssemblyName& parsedName = info.assembly_name();

        aname->name.m_value = il2cpp::utils::StringUtils::StringDuplicate(parsedName.name.c_str());
        if (utils::VmStringUtils::CaseInsensitiveEquals(parsedName.culture.c_str(), "neutral")) // culture names are case insensitive
            aname->culture.m_value = NULL;
        else
            aname->culture.m_value = il2cpp::utils::StringUtils::StringDuplicate(parsedName.culture.c_str());

        aname->hash_value.m_value = il2cpp::utils::StringUtils::StringDuplicate(parsedName.hash_value.c_str());
        aname->public_key.m_value = il2cpp::utils::StringUtils::StringDuplicate(parsedName.public_key.c_str());

        for (int i = 0; i < il2cpp::vm::kPublicKeyTokenLength; ++i)
            aname->public_key_token.padding[i] = parsedName.public_key_token[i];

        aname->hash_alg = parsedName.hash_alg;
        aname->hash_len = parsedName.hash_len;
        aname->flags = parsedName.flags;
        aname->major = parsedName.major;
        aname->minor = parsedName.minor;
        aname->build = parsedName.build;
        aname->revision = parsedName.revision;

        return true;
    }

#endif
} /* namespace Reflection */
} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
