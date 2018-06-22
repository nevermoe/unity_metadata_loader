#include "il2cpp-config.h"
#include "os/Environment.h"
#include "os/File.h"
#include "os/Image.h"
#include "os/Initialize.h"
#include "os/LibraryLoader.h"
#include "os/Locale.h"
#include "os/MemoryMappedFile.h"
#include "os/Path.h"
#include "NativeSymbol.h"
#include "utils/Collections.h"
#include "utils/PathUtils.h"
#include "utils/Runtime.h"
#include <string>

namespace il2cpp
{
namespace utils
{
#if IL2CPP_ENABLE_NATIVE_STACKTRACES

    static Il2CppMethodPointer MaskSpareBits(const Il2CppMethodPointer method)
    {
        return (Il2CppMethodPointer)((size_t)method & ~IL2CPP_POINTER_SPARE_BITS);
    }

    struct MethodInfoToMethodPointerConverter
    {
        Il2CppMethodPointer operator()(const MethodDefinitionKey& methodInfo) const
        {
            return MaskSpareBits(methodInfo.method);
        }
    };

    typedef il2cpp::utils::collections::ArrayValueMap<Il2CppMethodPointer, MethodDefinitionKey, MethodInfoToMethodPointerConverter> NativeMethodMap;
    static NativeMethodMap s_NativeMethods;

    void NativeSymbol::RegisterMethods(const std::vector<MethodDefinitionKey>& managedMethods)
    {
        s_NativeMethods.assign(managedMethods);
    }

#pragma pack(push, p1, 4)
    struct SymbolInfo
    {
        uint64_t address;
        uint32_t length;
    };
#pragma pack(pop, p1)

    static int32_t s_SymbolCount;
    static SymbolInfo* s_SymbolInfos;
    static void* s_ImageBase;

    static void* LoadSymbolInfoFileFrom(const std::string& path)
    {
        int error;
        il2cpp::os::FileHandle* handle = il2cpp::os::File::Open(path, kFileModeOpen, kFileAccessRead, kFileShareRead, kFileOptionsNone, &error);

        if (error != 0)
            return NULL;

        // Note that we won't unmap this file, we'll leave it open the entire lifetime of the process.
        void* mappedFile = il2cpp::os::MemoryMappedFile::Map(handle);

        il2cpp::os::File::Close(handle, &error);
        IL2CPP_ASSERT(error == 0);

        return mappedFile;
    }

    static void* LoadSymbolInfoFile()
    {
#if IL2CPP_TARGET_ANDROID
    #if defined(__i386__)
        std::string symbolMapFileName = "SymbolMap-x86";
    #else
        std::string symbolMapFileName = "SymbolMap-ARMv7";
    #endif
#else
    #if !IL2CPP_CAN_USE_MULTIPLE_SYMBOL_MAPS
        std::string symbolMapFileName = "SymbolMap";
    #elif IL2CPP_SIZEOF_VOID_P == 4
        std::string symbolMapFileName = "SymbolMap-32";
    #elif IL2CPP_SIZEOF_VOID_P == 8
        std::string symbolMapFileName = "SymbolMap-64";
    #else
        #error Unknown symbol map file name
    #endif
#endif

        void* result = LoadSymbolInfoFileFrom(il2cpp::utils::PathUtils::Combine(il2cpp::utils::PathUtils::DirectoryName(il2cpp::os::Path::GetExecutablePath()), symbolMapFileName));
        if (result != NULL)
            return result;

        return LoadSymbolInfoFileFrom(il2cpp::utils::PathUtils::Combine(utils::Runtime::GetDataDir(), symbolMapFileName));
    }

    static void InitializeSymbolInfos()
    {
        s_ImageBase = il2cpp::os::Image::GetImageBase();

        void* fileBuffer = LoadSymbolInfoFile();
        if (fileBuffer == NULL)
            return;

        s_SymbolCount = *((int32_t*)fileBuffer);
        s_SymbolInfos = (SymbolInfo*)((uint8_t*)fileBuffer + sizeof(s_SymbolCount));
    }

    static bool CompareEndOfSymbols(const SymbolInfo &a, const SymbolInfo &b)
    {
        return a.address + a.length < b.address + b.length;
    }

    static bool s_TriedToInitializeSymbolInfo = false;

    const VmMethod* NativeSymbol::GetMethodFromNativeSymbol(Il2CppMethodPointer nativeMethod)
    {
        if (!s_TriedToInitializeSymbolInfo)
        {
            // Only attempt to initialize the symbol information once. If it is not present the first time,
            // it likely won't be there later either. Repeated chcecking can cause performance problems.
            s_TriedToInitializeSymbolInfo = true;
            InitializeSymbolInfos();
        }

        // address has to be above our base address
        if ((void*)nativeMethod < (void*)s_ImageBase)
            return NULL;


        if (s_SymbolCount > 0)
        {
            SymbolInfo* end = s_SymbolInfos + s_SymbolCount;

            // our 'key' could be anywhere within a symbol. Our comparison function compares the end address
            // of the symbols. By doing this, upper bound returns the first symbol whose end address is greater
            // than our 'key'. This is our symbol since our end is the first end above an interior value.
            SymbolInfo interiorSymbol = { (size_t)((char*)nativeMethod - (char*)s_ImageBase), 0 };
            SymbolInfo* containingSymbol = std::upper_bound(s_SymbolInfos, end, interiorSymbol, &CompareEndOfSymbols);

            if (containingSymbol == end)
                return NULL;

            // We only include managed methods in the symbol data. A lookup for a native method might find the
            // previous or next managed method in the data. This will be incorrect, so check the start and the size,
            // to make sure the interior symbol is really within the method found in the containing symbol.
            if ((interiorSymbol.address != containingSymbol->address) &&
                ((interiorSymbol.address < containingSymbol->address) ||
                 (interiorSymbol.address - containingSymbol->address > containingSymbol->length)))
                return NULL;

            nativeMethod = (Il2CppMethodPointer)((char*)s_ImageBase + containingSymbol->address);

            // We can't assume that the map file is aligned.
            // We must use the same masking/no masking logic used to insert into the data structure for the lookup.
            // If we don't, the find will try to look up unmasked in a table full of masked values.

            // do exact lookup based on the symbol start address, as that is our key
            NativeMethodMap::iterator iter = s_NativeMethods.find_first(MaskSpareBits(nativeMethod));
            if (iter != s_NativeMethods.end())
            {
                return IL2CPP_VM_METHOD_METADATA_FROM_INDEX(iter->isGeneric, iter->methodIndex);
            }
        }
        else
        {
            // get the first symbol greater than the one we want
            NativeMethodMap::iterator iter = s_NativeMethods.upper_bound(nativeMethod);

            // This will cause it to fail to pickup the last method, but we cannot do anything about it
            if (iter != s_NativeMethods.begin() && iter != s_NativeMethods.end())
            {
                // go back one to get the symbol we actually want
                iter--;
                return IL2CPP_VM_METHOD_METADATA_FROM_INDEX(iter->isGeneric, iter->methodIndex);
            }
        }

        return NULL;
    }

#endif
} /* namespace utils */
} /* namespace il2cpp */
