#include "il2cpp-config.h"

#if IL2CPP_TARGET_WINRT || IL2CPP_TARGET_XBOXONE

#include "os/Win32/WindowsHeaders.h"
#include "Win32ApiSharedEmulation.h"

#include <io.h>
#include <windows.networking.connectivity.h>
#include <windows.storage.h>

using namespace il2cpp::winrt;
using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;
using namespace ABI::Windows::Foundation;
using namespace ABI::Windows::Foundation::Collections;
using namespace ABI::Windows::Networking;
using namespace ABI::Windows::Networking::Connectivity;
using namespace ABI::Windows::Storage;

extern "C"
{
BOOL WINAPI GetComputerNameW(LPWSTR lpBuffer, LPDWORD nSize)
{
#define ERROR_CHECK(hr) do { if (FAILED(hr)) { SetLastError(WIN32_FROM_HRESULT(hr)); return FALSE; } } while (false)

    ComPtr<INetworkInformationStatics> info;
    auto hr = RoGetActivationFactory(HStringReference(RuntimeClass_Windows_Networking_Connectivity_NetworkInformation).Get(), __uuidof(info), &info);
    ERROR_CHECK(hr);

    ComPtr<IVectorView<HostName*> > names;
    hr = info->GetHostNames(&names);
    ERROR_CHECK(hr);

    unsigned int size;
    hr = names->get_Size(&size);
    if (FAILED(hr) || !size)
    {
        SetLastError(WIN32_FROM_HRESULT(hr));
        return FALSE;
    }

    ComPtr<IHostName> name;
    hr = names->GetAt(0, &name);
    ERROR_CHECK(hr);

    HString displayName;
    hr = name->get_DisplayName(displayName.GetAddressOf());
    ERROR_CHECK(hr);

#undef ERROR_CHECK

    return CopyHStringToBuffer(displayName, lpBuffer, nSize);
}
} // extern "C"

#endif
