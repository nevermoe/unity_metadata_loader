#include "il2cpp-config.h"

#if IL2CPP_TARGET_WINRT

#include "os/Cryptography.h"
#include "os/Win32/WindowsHelpers.h"

#include <robuffer.h>
#include <windows.security.cryptography.h>
#include <wrl.h>

namespace il2cpp
{
namespace os
{

using namespace ABI::Windows::Security::Cryptography;
using namespace ABI::Windows::Storage::Streams;
using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;
using namespace Windows::Storage::Streams;

void* Cryptography::GetCryptographyProvider()
{
	ICryptographicBufferStatics* provider;

	auto hr = RoGetActivationFactory(HStringReference(RuntimeClass_Windows_Security_Cryptography_CryptographicBuffer).Get(), 
		__uuidof(ICryptographicBufferStatics), reinterpret_cast<void**>(&provider));

	if (SUCCEEDED(hr))
		return provider;

	return nullptr;
}

bool Cryptography::OpenCryptographyProvider()
{
	return false;
}

void Cryptography::ReleaseCryptographyProvider(void* provider)
{
	Assert(provider != nullptr);
	static_cast<ICryptographicBufferStatics*>(provider)->Release();
}

bool Cryptography::FillBufferWithRandomBytes(void* provider, uint32_t length, unsigned char* data)
{
	Assert(provider != nullptr);
	Assert(data != nullptr);

	ComPtr<IBuffer> buffer;
	ComPtr<IBufferByteAccess> bufferAccess;

	auto cryptoProvider = static_cast<ICryptographicBufferStatics*>(provider);	
	auto hr = cryptoProvider->GenerateRandom(length, &buffer);

	if (FAILED(hr))
		return false;

	hr = buffer.As(&bufferAccess);

	if (FAILED(hr))
		return false;

	uint8_t* bufferPtr;
	hr = bufferAccess->Buffer(&bufferPtr);

	if (FAILED(hr))
		return false;

	memcpy(data, bufferPtr, length);
	return true;
}

}
}

#endif 