#if IL2CPP_DEBUGGER_ENABLED

#include "transport-tcp.h"

#include <sstream>
#include <winsock2.h>
#include <ws2tcpip.h>

namespace il2cpp
{
namespace debugger
{
    struct TCPDataTransportHandle
    {
        int32_t listen_fd;
        int32_t client_fd;
        HANDLE send_mutex;

        TCPDataTransportHandle() :
            listen_fd(INVALID_SOCKET),
            client_fd(INVALID_SOCKET)
        {
            send_mutex = CreateMutex(0, false, L"send_mutex");
        }

        ~TCPDataTransportHandle()
        {
            CloseHandle(send_mutex);
            send_mutex = 0;
        }
    };
} /* namespace debugger */
} /* namespace il2cpp */


using namespace il2cpp::debugger;

TCPDataTransport::TCPDataTransport(const char *host, int32_t port) :
    _host(host),
    _port(port),
    _connected(false),
    _handle(new TCPDataTransportHandle())
{
}

TCPDataTransport::~TCPDataTransport()
{
    delete _handle;
    _handle = 0;
}

bool TCPDataTransport::Setup()
{
    int err;

    // Initialize Winsock

    WSADATA wsaData;
    err = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (err != 0)
    {
        LOG("WSAStartup failed with error: " << err);
        return false;
    }

    // Prepare connection
    std::stringstream oss;

    oss << _port;

    addrinfo hints;

    memset(&hints, 0, sizeof(struct addrinfo));

    hints.ai_flags = 0;
    hints.ai_protocol = 0;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    addrinfo *result;

    err = getaddrinfo(_host, oss.str().c_str(), &hints, &result);
    if (err != 0)
    {
        LOG("Unable to connect to " << _host << ":" << _port << ": " << gai_strerror(err));
        return false;
    }

    int socket_fd;

    for (addrinfo *rp = result; rp != NULL; rp = rp->ai_next)
    {
        socket_fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (socket_fd == INVALID_SOCKET)
            continue;

        err = bind(socket_fd, rp->ai_addr, rp->ai_addrlen);
        if (err != 0)
            continue;

        err = listen(socket_fd, 16);
        if (err != 0)
            continue;

        _handle->listen_fd = socket_fd;
        break;
    }

    freeaddrinfo(result);

    return true;
}

bool TCPDataTransport::WaitForClient()
{
    // Wait for a client to connect ..

    _handle->client_fd = accept(_handle->listen_fd, NULL, NULL);

    if (_handle->client_fd == INVALID_SOCKET)
    {
        LOG("Unable to listen on " << _handle->listen_fd);
        return false;
    }

    LOG("Accepted connection from client, connection fd=" << _handle->listen_fd);

    _connected = true;

    // Set the socket as non blocking
    int flag = 1;
    int err = setsockopt(_handle->client_fd, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(int));
    if (err != 0)
    {
        LOG("Error setting TCP_NODELAY.");
        Close();
        return false;
    }

    return true;
}

int32_t TCPDataTransport::Send(const uint8_t *data, size_t length, int flags)
{
    // Note: still not sure a mutex is needed here.
    WaitForSingleObject(_handle->send_mutex, INFINITE);
    int32_t result = send(_handle->client_fd, (const char*)data, length, flags);
    ReleaseMutex(_handle->send_mutex);
    return result;
}

int32_t TCPDataTransport::Recv(uint8_t *data, size_t length, int flags)
{
    int32_t read;
    size_t total_read = 0;

    do
    {
        read = recv(_handle->client_fd, (char*)data + total_read, length - total_read, flags);
        if (read > 0)
            total_read += read;
    }
    while ((read > 0 && total_read < length) || (read == -1 && errno == EINTR));

    return total_read;
}

void TCPDataTransport::Close()
{
    if (!_connected)
        return;

    shutdown(_handle->client_fd, SD_BOTH);
    shutdown(_handle->listen_fd, SD_BOTH);

    closesocket(_handle->client_fd);
    closesocket(_handle->listen_fd);

    _handle->listen_fd = INVALID_SOCKET;
    _handle->client_fd = INVALID_SOCKET;
    _connected = false;
}

#endif
