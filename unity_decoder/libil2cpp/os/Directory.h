#pragma once

#include <stdint.h>
#include <string>
#include <set>
#include "il2cpp-string-types.h"
#include "os/ErrorCodes.h"
#include "utils/StringView.h"

namespace il2cpp
{
namespace os
{
    class Directory
    {
    public:
        static std::string GetCurrent(int* error);
        static bool SetCurrent(const std::string& path, int* error);
        static bool Create(const std::string& path, int *error);
        static bool Remove(const std::string& path, int *error);
        static std::set<std::string> GetFileSystemEntries(const std::string& path, const std::string& pathWithPattern, int32_t attrs, int32_t mask, int* error);

        struct FindHandle
        {
            void* osHandle;
            Il2CppNativeString directoryPath;
            Il2CppNativeString pattern;

            FindHandle(const utils::StringView<Il2CppNativeChar>& searchPathWithPattern);
            ~FindHandle();

            inline void SetOSHandle(void* osHandle) { this->osHandle = osHandle; }
            int32_t CloseOSHandle();
        };

        static os::ErrorCode FindFirstFile(FindHandle* findHandle, const utils::StringView<Il2CppNativeChar>& searchPathWithPattern, Il2CppNativeString* resultFileName, int32_t* resultAttributes);
        static os::ErrorCode FindNextFile(FindHandle* findHandle, Il2CppNativeString* resultFileName, int32_t* resultAttributes);
    };
}
}
