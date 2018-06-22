#pragma once

#if !NET_4_0

#include "object-internals.h"

namespace il2cpp
{
namespace icalls
{
namespace System
{
namespace System
{
namespace Net
{
namespace NetworkInformation
{
    class LIBIL2CPP_CODEGEN_API LinuxNetworkInterface
    {
    public:
        static int32_t GetInterfaceAddresses(Il2CppIntPtr* ifap);
        static void FreeInterfaceAddresses(Il2CppIntPtr ifap);
        static void InitializeInterfaceAddresses();
    };
} // namespace NetworkInformation
} // namespace Net
} // namespace System
} // namespace System
} // namespace icalls
} // namespace il2cpp

#endif
