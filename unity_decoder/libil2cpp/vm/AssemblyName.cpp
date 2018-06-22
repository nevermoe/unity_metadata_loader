#include "il2cpp-config.h"
#include "vm/AssemblyName.h"
#include "vm/MetadataCache.h"
#include "vm/Runtime.h"
#include "tabledefs.h"
#include "class-internals.h"
#include "vm/Array.h"
#include "object-internals.h"
#include "vm/Reflection.h"
#include "vm/Type.h"
#include "vm/Object.h"
#include "utils/StringUtils.h"
#include "vm/String.h"
#include "vm/Class.h"

#include <vector>
#include <string>

namespace il2cpp
{
namespace vm
{
    static Il2CppObject* CreateVersion(uint32_t major, uint32_t minor, uint32_t build, uint32_t revision)
    {
        static const MethodInfo* versionContructor = NULL;
        if (!versionContructor)
            versionContructor = Class::GetMethodFromName(il2cpp_defaults.version, ".ctor", 4);

        Il2CppObject* version = Object::New(il2cpp_defaults.version);
        void* args[4] = { &major, &minor, &build, &revision };
        Runtime::Invoke(versionContructor, version, args, NULL);

        return version;
    }

    static Il2CppObject* CreateCulture(const char* cultureName)
    {
        static const MethodInfo* createCultureMethod = NULL;
        if (!createCultureMethod)
            createCultureMethod = Class::GetMethodFromName(il2cpp_defaults.culture_info, "CreateCulture", 2);

        bool reference = false;
        void* args[2];
        if (cultureName != NULL)
            args[0] = String::New(cultureName);
        else
            args[0] = String::New("neutral");
        args[1] = &reference;
        return Runtime::Invoke(createCultureMethod, NULL, args, NULL);
    }

    bool AssemblyName::ParseName(Il2CppReflectionAssemblyName* aname, std::string assemblyName)
    {
        il2cpp::vm::TypeNameParseInfo info;
        il2cpp::vm::TypeNameParser parser(assemblyName, info, false);

        if (!parser.ParseAssembly())
            return false;

        const il2cpp::vm::TypeNameParseInfo::AssemblyName& parsedName = info.assembly_name();
        IL2CPP_OBJECT_SETREF(aname, name, String::New(parsedName.name.c_str()));
        aname->major = parsedName.major;
        aname->minor = parsedName.minor;
        aname->build = parsedName.build;
        aname->revision = parsedName.revision;
        aname->flags = parsedName.flags;
        aname->hashalg = parsedName.hash_alg;

        IL2CPP_OBJECT_SETREF(aname, version, CreateVersion(parsedName.major, parsedName.minor, parsedName.build, parsedName.revision));
        IL2CPP_OBJECT_SETREF(aname, cultureInfo, CreateCulture(parsedName.culture.c_str()));

        if (parsedName.public_key_token[0])
        {
            IL2CPP_OBJECT_SETREF(aname, keyToken, Array::New(il2cpp_defaults.byte_class, kPublicKeyByteLength));
            char* p = il2cpp_array_addr(aname->keyToken, char, 0);

            char buf[2] = { 0 };
            for (int i = 0, j = 0; i < kPublicKeyByteLength; i++)
            {
                buf[0] = parsedName.public_key_token[j++];
                *p = (char)(strtol(buf, NULL, 16) << 4);
                buf[0] = parsedName.public_key_token[j++];
                *p |= (char)strtol(buf, NULL, 16);
                p++;
            }
        }
        else
            IL2CPP_OBJECT_SETREF(aname, keyToken, Array::New(il2cpp_defaults.byte_class, 0));

        return true;
    }

    static char HexValueToLowercaseAscii(uint8_t hexValue)
    {
        if (hexValue < 10)
            return char(hexValue + 48);

        return char(hexValue + 87);
    }

    static std::string PublicKeyTokenToString(const uint8_t* publicKeyToken)
    {
        std::string result(kPublicKeyByteLength * 2, '0');
        for (int i = 0; i < kPublicKeyByteLength; ++i)
        {
            uint8_t hi = (publicKeyToken[i] & 0xF0) >> 4;
            uint8_t lo = publicKeyToken[i] & 0x0F;

            result[i * 2] = HexValueToLowercaseAscii(hi);
            result[i * 2 + 1] = HexValueToLowercaseAscii(lo);
        }

        return result;
    }

    std::string AssemblyName::AssemblyNameToString(const Il2CppAssemblyName& aname)
    {
        std::string name;

        char buffer[1024];

        name += MetadataCache::GetStringFromIndex(aname.nameIndex);
        name += ", Version=";
        sprintf(buffer, "%d", aname.major);
        name += buffer;
        name += ".";
        sprintf(buffer, "%d", aname.minor);
        name += buffer;
        name += ".";
        sprintf(buffer, "%d", aname.build);
        name += buffer;
        name += ".";
        sprintf(buffer, "%d", aname.revision);
        name += buffer;
        name += ", Culture=";
        name += (aname.cultureIndex != kStringLiteralIndexInvalid ? MetadataCache::GetStringFromIndex(aname.cultureIndex) : "neutral");
        name += ", PublicKeyToken=";
        name += (aname.publicKeyToken[0] ? PublicKeyTokenToString(aname.publicKeyToken) : "null");
        name += ((aname.flags & ASSEMBLYREF_RETARGETABLE_FLAG) ? ", Retargetable=Yes" : "");

        return name;
    }
} /* namespace vm */
} /* namespace il2cpp */
