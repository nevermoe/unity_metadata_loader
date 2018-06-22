#if !NET_4_0

#include "il2cpp-config.h"
#include "LinuxNetworkInterface.h"
#include "external/xamarin-android/xamarin_getifaddrs.h"

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
    int32_t LinuxNetworkInterface::GetInterfaceAddresses(Il2CppIntPtr* ifap)
    {
        return _monodroid_getifaddrs(reinterpret_cast<_monodroid_ifaddrs**>(&ifap->m_value));
    }

    void LinuxNetworkInterface::FreeInterfaceAddresses(Il2CppIntPtr ifap)
    {
        _monodroid_freeifaddrs(static_cast<_monodroid_ifaddrs*>(ifap.m_value));
    }

    void LinuxNetworkInterface::InitializeInterfaceAddresses()
    {
        _monodroid_getifaddrs_init();
    }
} // namespace NetworkInformation
} // namespace Net
} // namespace System
} // namespace System
} // namespace icalls
} // namespace il2cpp

#endif
