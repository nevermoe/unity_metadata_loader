#include "il2cpp-config.h"

#include "icalls/System/System.Net.Sockets/SocketException.h"

#include "os/Error.h"

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
namespace Sockets
{
    int32_t SocketException::WSAGetLastError()
    {
        NOT_IMPLEMENTED_ICALL(SocketException::WSAGetLastError);

        return (int32_t)os::Error::GetLastError();
    }
} /* namespace Sockets */
} /* namespace Net */
} /* namespace System */
} /* namespace System */
} /* namespace icalls */
} /* namespace il2cpp */
