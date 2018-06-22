#include "il2cpp-config.h"
#include "il2cpp-api.h"
#include "vm/Array.h"
#include "os/Cryptography.h"
#include "utils/Memory.h"
#include "icalls/mscorlib/System.Security.Cryptography/RNGCryptoServiceProvider.h"

namespace il2cpp
{
namespace icalls
{
namespace mscorlib
{
namespace System
{
namespace Security
{
namespace Cryptography
{
    void RNGCryptoServiceProvider::RngClose(Il2CppIntPtr provider)
    {
        os::Cryptography::ReleaseCryptographyProvider(provider.m_value);
    }

    Il2CppIntPtr RNGCryptoServiceProvider::RngGetBytes(Il2CppIntPtr provider, Il2CppArray *data)
    {
        uint32_t len = il2cpp_array_length(data);
        unsigned char* buf = il2cpp_array_addr(data, unsigned char, 0);

        if (!os::Cryptography::FillBufferWithRandomBytes(provider.m_value, len, buf))
        {
            os::Cryptography::ReleaseCryptographyProvider(provider.m_value);
            provider = RngInitialize(NULL);

            if (!os::Cryptography::FillBufferWithRandomBytes(provider.m_value, len, buf))
            {
                os::Cryptography::ReleaseCryptographyProvider(provider.m_value);
                return Il2CppIntPtr::Zero;
            }
        }

        return provider;
    }

    Il2CppIntPtr RNGCryptoServiceProvider::RngInitialize(Il2CppArray *seed)
    {
        Il2CppIntPtr provider;
        provider.m_value = os::Cryptography::GetCryptographyProvider();

        if ((provider.m_value != 0) && seed)
        {
            uint32_t len = il2cpp_array_length(seed);
            unsigned char *buf = il2cpp_array_addr(seed, unsigned char, 0);
            unsigned char* data = (unsigned char*)IL2CPP_MALLOC(len);
            if (data)
            {
                memcpy(data, buf, len);
                os::Cryptography::FillBufferWithRandomBytes(provider.m_value, len, data);
                memset(data, 0, len);
                IL2CPP_FREE(data);
            }
        }

        return provider;
    }

    bool RNGCryptoServiceProvider::RngOpen()
    {
        return os::Cryptography::OpenCryptographyProvider();
    }
} /* namespace Cryptography */
} /* namespace Security */
} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
