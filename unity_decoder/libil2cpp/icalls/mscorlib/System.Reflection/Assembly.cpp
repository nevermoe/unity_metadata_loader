#include "il2cpp-config.h"
#include <stddef.h>
#include <vector>
#include <algorithm>
#include "icalls/mscorlib/System.Reflection/Assembly.h"
#include "icalls/mscorlib/System.Reflection/Module.h"
#include "utils/StringUtils.h"
#include "utils/PathUtils.h"
#include "os/File.h"
#include "os/MemoryMappedFile.h"
#include "os/Mutex.h"
#include "os/Path.h"
#include "utils/Memory.h"
#include "utils/Runtime.h"
#include "vm/Array.h"
#include "vm/Assembly.h"
#include "vm/AssemblyName.h"
#include "vm/Class.h"
#include "vm/Exception.h"
#include "vm/Field.h"
#include "vm/Image.h"
#include "vm/MetadataCache.h"
#include "vm/Object.h"
#include "vm/Reflection.h"
#include "vm/Runtime.h"
#include "vm/String.h"
#include "vm/Type.h"
#include "vm/Array.h"
#include "class-internals.h"
#include <limits>


using namespace il2cpp::vm;

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
    Il2CppString* Assembly::get_fullname(Il2CppReflectionAssembly *assembly)
    {
        return vm::String::New(vm::AssemblyName::AssemblyNameToString(assembly->assembly->aname).c_str());
    }

    Il2CppString*  Assembly::get_location(Il2CppReflectionAssembly *assembly)
    {
        NOT_IMPLEMENTED_ICALL_NO_ASSERT(Assembly::get_location, "Assembly::get_location is not functional on il2cpp");
        return vm::String::New("");
    }

    Il2CppReflectionAssembly* Assembly::GetEntryAssembly()
    {
        NOT_IMPLEMENTED_ICALL_NO_ASSERT(Assembly::GetEntryAssembly, "In the case of Unity this is always NULL. For a normal exe this is the assembly with Main.");
        return NULL;
    }

    Il2CppReflectionAssembly* Assembly::GetExecutingAssembly()
    {
        return vm::Reflection::GetAssemblyObject(MetadataCache::GetAssemblyFromIndex(vm::Image::GetExecutingImage()->assemblyIndex));
    }

#define CHECK_IF_NULL(v)    \
    if ( (v) == NULL && throwOnError ) \
        Exception::Raise (Exception::GetTypeLoadException ()); \
    if ( (v) == NULL ) \
        return NULL;

    Il2CppReflectionType* Assembly::InternalGetType(Il2CppReflectionAssembly *assembly, mscorlib_System_Reflection_Module *, Il2CppString* name, bool throwOnError, bool ignoreCase)
    {
        std::string str = utils::StringUtils::Utf16ToUtf8(utils::StringUtils::GetChars(name));

        il2cpp::vm::TypeNameParseInfo info;
        il2cpp::vm::TypeNameParser parser(str, info, false);

        if (!parser.Parse())
        {
            if (throwOnError)
                Exception::Raise(Exception::GetTypeLoadException());
            else
                return NULL;
        }

        CHECK_IF_NULL(assembly);

        Il2CppImage *image = (Il2CppImage*)vm::Assembly::GetImage(assembly->assembly);

        CHECK_IF_NULL(image);

        Il2CppClass *klass = Image::FromTypeNameParseInfo(image, info, ignoreCase);

        CHECK_IF_NULL(klass);

        il2cpp::vm::Class::Init(klass);

        const Il2CppType *type = Class::GetType(klass, info);

        CHECK_IF_NULL(type);

        return il2cpp::vm::Reflection::GetTypeObject(type);
    }

    Il2CppReflectionAssembly* Assembly::load_with_partial_name(Il2CppString* name, mscorlib_System_Security_Policy_Evidence*  evidence)
    {
        const Il2CppAssembly* assembly = vm::Assembly::GetLoadedAssembly(il2cpp::utils::StringUtils::Utf16ToUtf8(name->chars).c_str());
        if (assembly != NULL)
            return vm::Reflection::GetAssemblyObject(assembly);

        return NULL;
    }

    void Assembly::FillName(Il2CppReflectionAssembly * ass, mscorlib_System_Reflection_AssemblyName * aname)
    {
        Il2CppObject* assemblyNameObject = reinterpret_cast<Il2CppObject*>(aname);
        Il2CppClass* assemblyNameType = assemblyNameObject->klass;
        const Il2CppAssemblyName* assemblyName = &ass->assembly->aname;

        // System.Reflection.AssemblyName is not protected from stripping. Since this call will be used
        // very rarely, instead of including that type to stripper excludes, let's instead set fields only
        // if they're there.
        FieldInfo* assemblyNameField = Class::GetFieldFromName(assemblyNameType, "name");
        FieldInfo* codebaseField = Class::GetFieldFromName(assemblyNameType, "codebase");

        if (assemblyNameField != NULL)
            Field::SetValue(assemblyNameObject, assemblyNameField, String::New(MetadataCache::GetStringFromIndex(assemblyName->nameIndex)));

        if (codebaseField != NULL)
            Field::SetValue(assemblyNameObject, codebaseField, get_code_base(ass, false));

        FieldInfo* field = Class::GetFieldFromName(assemblyNameType, "major");
        if (field != NULL)
        {
            int32_t major = assemblyName->major;
            Field::SetValue(assemblyNameObject, field, &major);
        }

        field = Class::GetFieldFromName(assemblyNameType, "minor");
        if (field != NULL)
        {
            int32_t minor = assemblyName->minor;
            Field::SetValue(assemblyNameObject, field, &minor);
        }

        field = Class::GetFieldFromName(assemblyNameType, "build");
        if (field != NULL)
        {
            int32_t build = assemblyName->build;
            Field::SetValue(assemblyNameObject, field, &build);
        }

        field = Class::GetFieldFromName(assemblyNameType, "revision");
        if (field != NULL)
        {
            int32_t revision = assemblyName->revision;
            Field::SetValue(assemblyNameObject, field, &revision);
        }

        field = Class::GetFieldFromName(assemblyNameType, "cultureinfo");
        if (field != NULL)
        {
            Il2CppClass* cultureInfoType = Class::FromIl2CppType(field->type);
            FieldInfo* invariantCultureField = Class::GetFieldFromName(cultureInfoType, "invariant_culture_info");
            Il2CppObject* invariantCulture = NULL;

            if (invariantCultureField != NULL)
                Field::StaticGetValue(invariantCultureField, &invariantCulture);

            Field::SetValue(assemblyNameObject, field, invariantCulture);
        }

        field = Class::GetFieldFromName(assemblyNameType, "flags");
        if (field != NULL)
            Field::SetValue(assemblyNameObject, field, const_cast<void*>((const void*)&assemblyName->flags));

        field = Class::GetFieldFromName(assemblyNameType, "hashalg");
        if (field != NULL)
            Field::SetValue(assemblyNameObject, field, const_cast<void*>((const void*)&assemblyName->hash_alg));

        field = Class::GetFieldFromName(assemblyNameType, "keypair");
        if (field != NULL)
            Field::SetValue(assemblyNameObject, field, NULL);

        field = Class::GetFieldFromName(assemblyNameType, "publicKey");
        if (field != NULL)
            Field::SetValue(assemblyNameObject, field, Array::New(il2cpp_defaults.byte_class, 0));

        field = Class::GetFieldFromName(assemblyNameType, "keyToken");
        if (field != NULL)
        {
            Il2CppArray* keyTokenManaged = NULL;

            // Set it to non-null only if public key token is not all zeroes
            for (int i = 0; i < kPublicKeyByteLength; i++)
            {
                if (assemblyName->publicKeyToken[i] != 0)
                {
                    keyTokenManaged = Array::New(il2cpp_defaults.byte_class, kPublicKeyByteLength);
                    memcpy(il2cpp::vm::Array::GetFirstElementAddress(keyTokenManaged), assemblyName->publicKeyToken, kPublicKeyByteLength);
                    break;
                }
            }

            Field::SetValue(assemblyNameObject, field, keyTokenManaged);
        }

        field = Class::GetFieldFromName(assemblyNameType, "versioncompat");
        if (field != NULL)
        {
            int32_t kSameProcess = 2;
            Field::SetValue(assemblyNameObject, field, &kSameProcess);
        }

        field = Class::GetFieldFromName(assemblyNameType, "version");
        if (field != NULL)
        {
            Il2CppClass* versionType = Class::FromIl2CppType(field->type);
            Il2CppObject* version = Object::New(versionType);

            FieldInfo* versionField = Class::GetFieldFromName(versionType, "_Major");
            if (versionField != NULL)
            {
                int32_t major = assemblyName->major;
                Field::SetValue(version, versionField, &major);
            }

            versionField = Class::GetFieldFromName(versionType, "_Minor");
            if (versionField != NULL)
            {
                int32_t minor = assemblyName->minor;
                Field::SetValue(version, versionField, &minor);
            }

            versionField = Class::GetFieldFromName(versionType, "_Build");
            if (versionField != NULL)
            {
                int32_t build = assemblyName->build;
                Field::SetValue(version, versionField, &build);
            }

            versionField = Class::GetFieldFromName(versionType, "_Revision");
            if (versionField != NULL)
            {
                int32_t revision = assemblyName->revision;
                Field::SetValue(version, versionField, &revision);
            }

            Field::SetValue(assemblyNameObject, field, version);
        }

        field = Class::GetFieldFromName(assemblyNameType, "processor_architecture");
        if (field != NULL)
        {
            int32_t kMSILArchitecture = 1;
            Field::SetValue(assemblyNameObject, field, &kMSILArchitecture);
        }
    }

    Il2CppArray* Assembly::GetModulesInternal(Il2CppReflectionAssembly * thisPtr)
    {
        Il2CppArray* arr = vm::Array::New(il2cpp_defaults.module_class, 1);
        il2cpp_array_setref(arr, 0, vm::Reflection::GetModuleObject(vm::Assembly::GetImage(thisPtr->assembly)));
        return arr;
    }

    bool Assembly::LoadPermissions(mscorlib_System_Reflection_Assembly * a, Il2CppIntPtr* minimum, int32_t* minLength, Il2CppIntPtr* optional, int32_t* optLength, Il2CppIntPtr* refused, int32_t* refLength)
    {
        NOT_IMPLEMENTED_ICALL(Assembly::LoadPermissions);
        return false;
    }

    Il2CppReflectionAssembly* Assembly::GetCallingAssembly()
    {
        return vm::Reflection::GetAssemblyObject(MetadataCache::GetAssemblyFromIndex(Image::GetCallingImage()->assemblyIndex));
    }

    Il2CppString* Assembly::get_code_base(Il2CppReflectionAssembly * assembly, bool escaped)
    {
        std::string executableDirectory = utils::PathUtils::DirectoryName(os::Path::GetExecutablePath());
        std::replace(executableDirectory.begin(), executableDirectory.end(), '\\', '/');
        return vm::String::New(utils::StringUtils::Printf("file://%s/%s.dll", executableDirectory.c_str(), MetadataCache::GetStringFromIndex(assembly->assembly->aname.nameIndex)).c_str());
    }

    Il2CppArray* Assembly::GetTypes(Il2CppReflectionAssembly* thisPtr, bool exportedOnly)
    {
        const Il2CppImage* image = MetadataCache::GetImageFromIndex(thisPtr->assembly->imageIndex);
        return Module::InternalGetTypes(vm::Reflection::GetModuleObject(image));
    }

    Il2CppString* Assembly::InternalImageRuntimeVersion(Il2CppAssembly* self)
    {
        NOT_SUPPORTED_IL2CPP(Assembly::InternalImageRuntimeVersion, "This icall is not supported by il2cpp.");

        return 0;
    }

    Il2CppReflectionMethod* Assembly::get_EntryPoint(Il2CppReflectionAssembly* self)
    {
        const MethodInfo* method = Image::GetEntryPoint(MetadataCache::GetImageFromIndex(self->assembly->imageIndex));
        if (method == NULL)
            return NULL;

        return il2cpp::vm::Reflection::GetMethodObject(method, NULL);
    }

    bool Assembly::get_global_assembly_cache(Il2CppAssembly* self)
    {
        return false;
    }

    Il2CppObject* Assembly::GetFilesInternal(Il2CppAssembly* self, Il2CppString* name, bool getResourceModules)
    {
        // Some code paths in mscorlib (e.g. Encoding.GetEncoding) will expect this icall to return NULL. If it
        // instead throws a NotSupportedException, the mscorlib code path changes, and we see some IL2CPP-specific bugs.
        return NULL;
    }

    void Assembly::InternalGetAssemblyName(Il2CppString* assemblyFile, Il2CppAssemblyName* aname)
    {
        NOT_SUPPORTED_IL2CPP(Assembly::InternalGetAssemblyName, "This icall is not supported by il2cpp.");
    }

    Il2CppReflectionAssembly* Assembly::LoadFrom(Il2CppString* assemblyFile, bool refonly)
    {
        assert(!refonly && "This icall is not supported by il2cpp when refonly=true");

        // Our implementation is going to behave a bit different.  We can't actually load any assembly.  If we didn't know about the assembly at conversion time,
        // then we won't be able to do anything.
        // On the other hand, if the name of the assembly matches the name of an assembly that we converted, then lets return the assembly that we know about.
        std::string utf8Path = utils::StringUtils::Utf16ToUtf8(utils::StringUtils::GetChars(assemblyFile));
        std::string fileName = utils::PathUtils::BasenameNoExtension(utf8Path);

        const Il2CppAssembly* foundAssembly = MetadataCache::GetAssemblyByName(fileName);

        if (!foundAssembly)
        {
            vm::Exception::Raise(vm::Exception::GetFileLoadException(fileName.c_str()));
            IL2CPP_UNREACHABLE;
        }

        return vm::Reflection::GetAssemblyObject(foundAssembly);
    }

    Il2CppArray* Assembly::GetNamespaces(Il2CppAssembly* self)
    {
        NOT_SUPPORTED_IL2CPP(Assembly::GetNamespaces, "This icall is not supported by il2cpp.");

        return 0;
    }

    Il2CppArray* Assembly::GetReferencedAssemblies(Il2CppReflectionAssembly* self)
    {
        vm::AssemblyNameVector referencedAssemblies;
        vm::Assembly::GetReferencedAssemblies(self->assembly, &referencedAssemblies);
        Il2CppArray* result = Array::New(il2cpp_defaults.assembly_name_class, (il2cpp_array_size_t)referencedAssemblies.size());
        size_t index = 0;
        for (vm::AssemblyNameVector::const_iterator aname = referencedAssemblies.begin(); aname != referencedAssemblies.end(); ++aname)
        {
            Il2CppReflectionAssemblyName* reflectionAssemblyName = vm::Reflection::GetAssemblyNameObject(*aname);
            il2cpp_array_set(result, Il2CppReflectionAssemblyName*, index, reflectionAssemblyName);
            index++;
        }

        return result;
    }

    static void* LoadResourceFile(Il2CppReflectionAssembly* assembly)
    {
        std::string resourcesDirectory = utils::PathUtils::Combine(utils::Runtime::GetDataDir(), utils::StringView<char>("Resources"));

        std::string resourceFileName(MetadataCache::GetImageFromIndex(assembly->assembly->imageIndex)->name);
        resourceFileName += "-resources.dat";

        std::string resourceFilePath = utils::PathUtils::Combine(resourcesDirectory, resourceFileName);

        int error = 0;
        os::FileHandle* handle = os::File::Open(resourceFilePath, kFileModeOpen, kFileAccessRead, kFileShareRead, kFileOptionsNone, &error);
        if (error != 0)
            return NULL;

        void* fileBuffer = os::MemoryMappedFile::Map(handle);

        os::File::Close(handle, &error);
        if (error != 0)
        {
            os::MemoryMappedFile::Unmap(fileBuffer);
            fileBuffer = NULL;
            return NULL;
        }

        return fileBuffer;
    }

    static os::Mutex s_ResourceDataMutex;

    static void* LoadResourceData(Il2CppReflectionAssembly* assembly, EmbeddedResourceRecord record)
    {
        os::AutoLock lock(&s_ResourceDataMutex);

        void* resourceData = vm::Image::GetCachedResourceData(record.image, record.name);
        if (resourceData != NULL)
            return resourceData;

        void* fileBuffer = vm::Image::GetCachedMemoryMappedResourceFile(assembly);
        if (fileBuffer == NULL)
        {
            fileBuffer = LoadResourceFile(assembly);
            if (fileBuffer == NULL)
                return NULL;

            vm::Image::CacheMemoryMappedResourceFile(assembly, fileBuffer);
        }

        resourceData = (uint8_t*)fileBuffer + record.offset;

        vm::Image::CacheResourceData(record, resourceData);

        return resourceData;
    }

    static int ReadFromBuffer(uint8_t* buffer, int offset, int size, void* output)
    {
        memcpy(output, buffer + offset, size);

        return size;
    }

    static std::vector<EmbeddedResourceRecord> GetResourceRecords(Il2CppReflectionAssembly* assembly)
    {
        std::vector<EmbeddedResourceRecord> resourceRecords;

        void* fileBuffer = vm::Image::GetCachedMemoryMappedResourceFile(assembly);
        if (fileBuffer == NULL)
        {
            fileBuffer = LoadResourceFile(assembly);
            if (fileBuffer == NULL)
                return resourceRecords;

            vm::Image::CacheMemoryMappedResourceFile(assembly, fileBuffer);
        }

        int32_t resourceRecordsSize = 0;
        uint32_t bytesRead = ReadFromBuffer((uint8_t*)fileBuffer, 0, sizeof(int32_t), &resourceRecordsSize);

        int32_t currentResourceDataOffset = bytesRead + resourceRecordsSize;

        int32_t numberOfResources = 0;
        bytesRead += ReadFromBuffer((uint8_t*)fileBuffer, bytesRead, sizeof(int32_t), &numberOfResources);
        for (int resourceIndex = 0; resourceIndex < numberOfResources; ++resourceIndex)
        {
            uint32_t resourceDataSize = 0;
            bytesRead += ReadFromBuffer((uint8_t*)fileBuffer, bytesRead, sizeof(int32_t), &resourceDataSize);

            int32_t resourceNameSize = 0;
            bytesRead += ReadFromBuffer((uint8_t*)fileBuffer, bytesRead, sizeof(int32_t), &resourceNameSize);

            std::vector<char> resourceName(resourceNameSize);
            bytesRead += ReadFromBuffer((uint8_t*)fileBuffer, bytesRead, resourceNameSize, &resourceName[0]);

            resourceRecords.push_back(EmbeddedResourceRecord(MetadataCache::GetImageFromIndex(assembly->assembly->imageIndex), std::string(resourceName.begin(), resourceName.end()), currentResourceDataOffset, resourceDataSize));

            currentResourceDataOffset += resourceDataSize;
        }

        return resourceRecords;
    }

    Il2CppArray* Assembly::GetManifestResourceNames(Il2CppReflectionAssembly* assembly)
    {
        std::vector<EmbeddedResourceRecord> resourceRecords = GetResourceRecords(assembly);

        IL2CPP_ASSERT(resourceRecords.size() <= static_cast<size_t>(std::numeric_limits<il2cpp_array_size_t>::max()));
        Il2CppArray* resourceNameArray = vm::Array::New(il2cpp_defaults.string_class, static_cast<il2cpp_array_size_t>(resourceRecords.size()));
        for (size_t i = 0; i < resourceRecords.size(); ++i)
            il2cpp_array_setref(resourceNameArray, i, vm::String::New(resourceRecords[i].name.c_str()));

        return resourceNameArray;
    }

    class ResourceNameMatcher
    {
    public:
        ResourceNameMatcher(const std::string& resourceNameToFind) : needle(resourceNameToFind)
        {}

        bool operator()(const EmbeddedResourceRecord& data) const
        {
            return data.name == needle;
        }

    private:
        std::string needle;
    };

    bool Assembly::GetManifestResourceInfoInternal(Il2CppReflectionAssembly* assembly, Il2CppString* name, Il2CppManifestResourceInfo* info)
    {
        std::vector<EmbeddedResourceRecord> resourceRecords = GetResourceRecords(assembly);
        if (std::find_if(resourceRecords.begin(), resourceRecords.end(), ResourceNameMatcher(utils::StringUtils::Utf16ToUtf8(name->chars))) != resourceRecords.end())
        {
            info->location = RESOURCE_LOCATION_EMBEDDED | RESOURCE_LOCATION_IN_MANIFEST;

            NOT_IMPLEMENTED_ICALL_NO_ASSERT(Assembly::GetManifestResourceInfoInternal, "We have not yet implemented file or assembly resources.");

            return true;
        }

        return false;
    }

    Il2CppIntPtr Assembly::GetManifestResourceInternal(Il2CppReflectionAssembly* assembly, Il2CppString* name, int* size, Il2CppReflectionModule** module)
    {
        std::vector<EmbeddedResourceRecord> resourceRecords = GetResourceRecords(assembly);
        std::vector<EmbeddedResourceRecord>::iterator resource = std::find_if(resourceRecords.begin(), resourceRecords.end(), ResourceNameMatcher(utils::StringUtils::Utf16ToUtf8(name->chars)));
        if (resource != resourceRecords.end())
        {
            *module = vm::Reflection::GetModuleObject(MetadataCache::GetImageFromIndex(assembly->assembly->imageIndex));
            *size = resource->size;
            Il2CppIntPtr result;
            result.m_value = LoadResourceData(assembly, *resource);
            return result;
        }

        return Il2CppIntPtr::Zero;
    }

    int32_t Assembly::MonoDebugger_GetMethodToken(void* /* System.Reflection.MethodBase */ method)
    {
        NOT_SUPPORTED_IL2CPP(Assembly::MonoDebugger_GetMethodToken, "This icall is not supported by il2cpp.");

        return 0;
    }

    Il2CppReflectionModule* Assembly::GetManifestModuleInternal(Il2CppAssembly* self)
    {
        NOT_SUPPORTED_IL2CPP(Assembly::GetManifestModuleInternal, "This icall is not supported by il2cpp.");

        return 0;
    }

    bool Assembly::get_ReflectionOnly(Il2CppAssembly* self)
    {
        // It doesn't mean anything to have a reflection only assembly in il2cpp since we can't load a managed assembly that we didn't convert.  So let's always return false.
        return false;
    }

#if NET_4_0
    Il2CppString* Assembly::GetAotId()
    {
        return NULL;
    }

#endif
} /* namespace Reflection */
} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
