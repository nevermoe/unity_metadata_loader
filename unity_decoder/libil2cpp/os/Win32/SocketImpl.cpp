#include "il2cpp-config.h"

#if !IL2CPP_USE_GENERIC_SOCKET_IMPL && IL2CPP_TARGET_WINDOWS

#include <time.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "WindowsHelpers.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#if !IL2CPP_TARGET_XBOXONE
    #include <mswsock.h>
#endif
#include "os/Error.h"
#include "os/Socket.h"
#include "os/ErrorCodes.h"
#include "os/Win32/SocketImpl.h"
#include "utils/StringUtils.h"

#include "il2cpp-vm-support.h"

typedef int socklen_t;

namespace il2cpp
{
namespace os
{
    static bool is_loopback(int32_t family, uint8_t *addr)
    {
        return ((family) == AF_INET ? ((addr)[0] == 127) : false);
    }

    static struct in_addr *get_local_ips(int32_t family, int32_t *nips)
    {
        *nips = 0;

        return 0;
    }

    static bool hostent_get_info(struct hostent *he, std::string &name, std::vector<std::string> &aliases, std::vector<std::string> &addr_list)
    {
        if (he == NULL)
            return false;

        if (he->h_length != 4 || he->h_addrtype != AF_INET)
            return false;

        name.assign(he->h_name);

        for (int32_t i = 0; he->h_aliases[i] != NULL; ++i)
            aliases.push_back(he->h_aliases[i]);

        if (he->h_addrtype == AF_INET)
        {
            struct in_addr addr = {0};
            for (int32_t i = 0; he->h_addr_list[i] != NULL; ++i)
            {
                addr.s_addr = *(u_long*)he->h_addr_list[i];
                addr_list.push_back(inet_ntoa(addr));
            }
        }
        // TODO(gab): add support for IPv6
        //else if(he->h_addrtype == AF_INET6)

        return true;
    }

    static bool hostent_get_info_with_local_ips(struct hostent *he, std::string &name, std::vector<std::string> &aliases, std::vector<std::string> &addr_list)
    {
        int32_t i = 0;
        int32_t nlocal_in = 0;

        if (he != NULL)
        {
            if (he->h_length != 4 || he->h_addrtype != AF_INET)
                return false;

            name.assign(he->h_name);

            for (int32_t i = 0; he->h_aliases[i] != NULL; ++i)
                aliases.push_back(he->h_aliases[i]);
        }

        struct in_addr *local_in = get_local_ips(AF_INET, &nlocal_in);

        if (nlocal_in)
        {
            for (int32_t i = 0; i < nlocal_in; ++i)
                addr_list.push_back(inet_ntoa(local_in[i]));

            free(local_in);
        }
        else if (he == NULL)
        {
            // If requesting "" and there are no other interfaces up, MS returns 127.0.0.1
            addr_list.push_back("127.0.0.1");
            return true;
        }

        if (nlocal_in == 0 && he != NULL)
        {
            if (he->h_addrtype == AF_INET)
            {
                struct in_addr addr = {0};
                for (int32_t i = 0; he->h_addr_list[i] != NULL; ++i)
                {
                    addr.s_addr = *(u_long*)he->h_addr_list[i];
                    addr_list.push_back(inet_ntoa(addr));
                }
            }
            // TODO(gab): add support for IPv6
            //else if(he->h_addrtype == AF_INET6)
        }

        return true;
    }

    static int32_t convert_socket_flags(os::SocketFlags flags)
    {
        int32_t c_flags = 0;

        if (flags)
        {
            // Check if contains invalid flag values
            if (flags & ~(os::kSocketFlagsOutOfBand | os::kSocketFlagsMaxIOVectorLength | os::kSocketFlagsPeek | os::kSocketFlagsDontRoute | os::kSocketFlagsPartial))
            {
                return -1;
            }

            if (flags & os::kSocketFlagsOutOfBand)
                c_flags |= MSG_OOB;

            if (flags & os::kSocketFlagsPeek)
                c_flags |= MSG_PEEK;

            if (flags & os::kSocketFlagsDontRoute)
                c_flags |= MSG_DONTROUTE;

            // Ignore Partial - see bug 349688.  Don't return -1, because
            // according to the comment in that bug ms runtime doesn't for
            // UDP sockets (this means we will silently ignore it for TCP
            // too)

    #ifdef MSG_MORE
            if (flags & os::kSocketFlagsPartial)
                c_flags |= MSG_MORE;
    #endif
        }

        return c_flags;
    }

    WaitStatus SocketImpl::GetHostByAddr(const std::string &address, std::string &name, std::vector<std::string> &aliases, std::vector<std::string> &addr_list)
    {
        struct in_addr inaddr;

        if (inet_pton(AF_INET, address.c_str(), &inaddr) <= 0)
            return kWaitStatusFailure;

        struct hostent *he = gethostbyaddr((char*)&inaddr, sizeof(inaddr), AF_INET);

        if (he == NULL)
        {
            name = address;
            addr_list.push_back(name);

            return kWaitStatusSuccess;
        }

        return hostent_get_info(he, name, aliases, addr_list)
            ? kWaitStatusSuccess
            : kWaitStatusFailure;
    }

    WaitStatus SocketImpl::GetHostByName(const std::string &host, std::string &name, std::vector<std::string> &aliases, std::vector<std::string> &addr_list)
    {
        char this_hostname[256] = {0};

        const char *hostname = host.c_str();
        bool add_local_ips = (*hostname == '\0');

        if (!add_local_ips && gethostname(this_hostname, sizeof(this_hostname)) != -1)
        {
            if (!strcmp(hostname, this_hostname))
                add_local_ips = true;
        }

        struct hostent *he = NULL;
        if (*hostname)
            he = gethostbyname(hostname);

        if (*hostname && he == NULL)
            return kWaitStatusFailure;

        return (add_local_ips
                ? hostent_get_info_with_local_ips(he, name, aliases, addr_list)
                : hostent_get_info(he, name, aliases, addr_list))
            ? kWaitStatusSuccess
            : kWaitStatusFailure;
    }

    void SocketImpl::Startup()
    {
        WSADATA wsadata;

        const int32_t ret = WSAStartup(MAKEWORD(2, 0), &wsadata);
        if (ret != 0)
        {
            exit(-1);
        }
    }

    void SocketImpl::Cleanup()
    {
        WSACleanup();
    }

    WaitStatus SocketImpl::GetHostName(std::string &name)
    {
        char hostname[256];
        int32_t ret = gethostname(hostname, sizeof(hostname));

        if (ret == -1)
            return kWaitStatusFailure;

        name.assign(hostname);

        return kWaitStatusSuccess;
    }

    SocketImpl::SocketImpl(ThreadStatusCallback thread_status_callback)
        :   _is_valid(false)
        ,   _fd(-1)
        ,   _domain(-1)
        ,   _type(-1)
        ,   _protocol(-1)
        ,   _saved_error(kErrorCodeSuccess)
        ,   _still_readable(0)
        ,   _thread_status_callback(thread_status_callback)
    {
    }

    SocketImpl::~SocketImpl()
    {
    }

    static int32_t convert_address_family(AddressFamily family)
    {
        switch (family)
        {
            case kAddressFamilyUnspecified:
                return AF_UNSPEC;

            // NOTE(gab): not supported on Windows
            //case kAddressFamilyUnix:
            //  return AF_UNIX;

            case kAddressFamilyInterNetwork:
                return AF_INET;

            case kAddressFamilyIpx:
                return AF_IPX;

            case kAddressFamilySna:
                return AF_SNA;

            case kAddressFamilyDecNet:
                return AF_DECnet;

            case kAddressFamilyAppleTalk:
                return AF_APPLETALK;

            case kAddressFamilyInterNetworkV6:
                return AF_INET6;

#ifdef AF_IRDA
            case kAddressFamilyIrda:
                return AF_IRDA;
#endif

            default:
                break;
        }

        return -1;
    }

    static int32_t convert_socket_type(SocketType type)
    {
        switch (type)
        {
            case kSocketTypeStream:
                return SOCK_STREAM;

            case kSocketTypeDgram:
                return SOCK_DGRAM;

            case kSocketTypeRaw:
                return SOCK_RAW;

            case kSocketTypeRdm:
                return SOCK_RDM;

            case kSocketTypeSeqpacket:
                return SOCK_SEQPACKET;

            default:
                break;
        }

        return -1;
    }

    static int32_t convert_socket_protocol(ProtocolType protocol)
    {
        switch (protocol)
        {
            case kProtocolTypeIP:
            case kProtocolTypeIPv6:
            case kProtocolTypeIcmp:
            case kProtocolTypeIgmp:
            case kProtocolTypeGgp:
            case kProtocolTypeTcp:
            case kProtocolTypePup:
            case kProtocolTypeUdp:
            case kProtocolTypeIdp:
                // In this case the enum values map exactly.
                return (int32_t)protocol;

            default:
                break;
        }

        // Everything else in unsupported and unexpected
        return -1;
    }

    static int SocketExceptionFilter(unsigned int code)
    {
        // Sometimes, we call the socket functions and close the socket right after,
        // and in some rare cases, it throws EXCEPTION_INVALID_HANDLE SEH exception
        // rather than returning an error code. Although this is undocumented on MSDN,
        // it causes a crash just because it thinks we gave it an invalid handle.
        // We guard against it by wrapping every socket call with __try/__except
        if (code == EXCEPTION_INVALID_HANDLE)
            return EXCEPTION_CONTINUE_EXECUTION;

        return EXCEPTION_CONTINUE_SEARCH;
    }

    WaitStatus SocketImpl::Create(AddressFamily family, SocketType type, ProtocolType protocol)
    {
        _fd = -1;
        _is_valid = false;
        _still_readable = 1;
        _domain = convert_address_family(family);
        _type = convert_socket_type(type);
        _protocol = convert_socket_protocol(protocol);

        IL2CPP_ASSERT(_type != -1 && "Unsupported socket type");
        IL2CPP_ASSERT(_domain != -1 && "Unsupported address family");
        IL2CPP_ASSERT(_protocol != -1 && "Unsupported protocol type");

        _fd = socket(_domain, _type, _protocol);
        if (_fd == -1 && _domain == AF_INET && _type == SOCK_RAW && _protocol == 0)
        {
            // Retry with protocol == 4 (see bug #54565)
            _protocol = 4;
            _fd = socket(AF_INET, SOCK_RAW, 4);
        }

        if (_fd == -1)
        {
            StoreLastError();

            return kWaitStatusFailure;
        }

        // if (fd >= _wapi_fd_reserve)
        // {
        //  WSASetLastError (WSASYSCALLFAILURE);
        //  closesocket (fd);

        //  return(INVALID_SOCKET);
        // }

        /* .net seems to set this by default for SOCK_STREAM, not for
         * SOCK_DGRAM (see bug #36322)
         *
         * It seems winsock has a rather different idea of what
         * SO_REUSEADDR means.  If it's set, then a new socket can be
         * bound over an existing listening socket.  There's a new
         * windows-specific option called SO_EXCLUSIVEADDRUSE but
         * using that means the socket MUST be closed properly, or a
         * denial of service can occur.  Luckily for us, winsock
         * behaves as though any other system would when SO_REUSEADDR
         * is true, so we don't need to do anything else here.  See
         * bug 53992.
         */
        {
            int32_t v = 1;
            const int32_t ret = setsockopt((SOCKET)_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&v, sizeof(v));

            if (ret == -1)
            {
                if (closesocket((SOCKET)_fd) == -1)
                    StoreLastError();

                return kWaitStatusFailure;
            }
        }

        // mono_once (&socket_ops_once, socket_ops_init);

        // handle = _wapi_handle_new_fd (WAPI_HANDLE_SOCKET, fd, &socket_handle);
        // if (handle == _WAPI_HANDLE_INVALID) {
        //  g_warning ("%s: error creating socket handle", __func__);
        //  WSASetLastError (WSASYSCALLFAILURE);
        //  closesocket (fd);

        //  return(INVALID_SOCKET);
        // }

        _is_valid = true;

        return kWaitStatusSuccess;
    }

    WaitStatus SocketImpl::Create(SocketDescriptor fd, int32_t family, int32_t type, int32_t protocol)
    {
        _fd = fd;
        _is_valid = (fd != -1);
        _still_readable = 1;
        _domain = family;
        _type = type;
        _protocol = protocol;

        IL2CPP_ASSERT(_type != -1 && "Unsupported socket type");
        IL2CPP_ASSERT(_domain != -1 && "Unsupported address family");
        IL2CPP_ASSERT(_protocol != -1 && "Unsupported protocol type");

        return kWaitStatusSuccess;
    }

    WaitStatus SocketImpl::Close()
    {
        _saved_error = kErrorCodeSuccess;

        SOCKET fd = (SOCKET)_fd;
        if (_is_valid && fd != -1)
        {
            if (closesocket(fd) == -1)
                StoreLastError();
        }

        _fd = -1;
        _is_valid = false;
        _still_readable = 0;
        _domain = -1;
        _type = -1;
        _protocol = -1;

        return kWaitStatusSuccess;
    }

    WaitStatus SocketImpl::SetBlocking(bool blocking)
    {
        /*
        * block == TRUE/FALSE means we will block/not block.
        * But the ioctlsocket call takes TRUE/FALSE for non-block/block
        */
        u_long ioctl_socket_blocking = (u_long) !blocking;

        SOCKET fd = (SOCKET)_fd;
        if (fd == -1)
        {
            Error::SetLastError(il2cpp::os::kWSAeshutdown);
            return kWaitStatusFailure;
        }

        int32_t ret = -1;

        __try
        {
            ret = ioctlsocket(fd, FIONBIO, &ioctl_socket_blocking);
        }
        __except (SocketExceptionFilter(GetExceptionCode()))
        {
        }

        if (ret == SOCKET_ERROR)
        {
            StoreLastError();

            return kWaitStatusFailure;
        }

        return kWaitStatusSuccess;
    }

    ErrorCode SocketImpl::GetLastError() const
    {
        return _saved_error;
    }

    void SocketImpl::StoreLastError()
    {
        const ErrorCode error = (ErrorCode)WSAGetLastError();

        Error::SetLastError(error);

        _saved_error = error;
    }

    void SocketImpl::StoreLastError(int32_t error_no)
    {
        const ErrorCode error = (ErrorCode)error_no;

        Error::SetLastError(error);

        _saved_error = error;
    }

    static void sockaddr_from_address(uint32_t address, uint16_t port, struct sockaddr *sa, socklen_t *sa_size)
    {
        struct sockaddr_in sa_in = {0};

        sa_in.sin_family = AF_INET;
        sa_in.sin_port = port;
        sa_in.sin_addr.s_addr = address;

        *sa_size = sizeof(struct sockaddr_in);
        *sa = *((struct sockaddr*)&sa_in);
    }

    static bool socketaddr_to_endpoint_info(const struct sockaddr *address, socklen_t address_len, EndPointInfo &info)
    {
        info.family = (os::AddressFamily)address->sa_family;

        if (info.family == os::kAddressFamilyInterNetwork)
        {
            const struct sockaddr_in *address_in = (const struct sockaddr_in *)address;

            info.data.inet.port = ntohs(address_in->sin_port);
            info.data.inet.address = ntohl(address_in->sin_addr.s_addr);

            return true;
        }

        // NOTE(gab): kAddressFamilyUnix not supported on Windows
        //if (info.family == os::kAddressFamilyUnix)
        //{
        //  for (int32_t i = 0; i < address_len; i++)
        //      info.data.path[i] = address->sa_data[i];

        //  return true;
        //}

        return false;
    }

    WaitStatus SocketImpl::Bind(const char *path)
    {
        // NOTE(gab): not supported on Windows.
        return kWaitStatusFailure;
    }

    WaitStatus SocketImpl::Bind(const char *address, uint16_t port)
    {
        struct sockaddr sa = {0};
        socklen_t sa_size = 0;

        sockaddr_from_address(inet_addr(address), htons(port), &sa, &sa_size);

        SOCKET fd = (SOCKET)_fd;
        if (fd == -1)
        {
            Error::SetLastError(il2cpp::os::kWSAeshutdown);
            return kWaitStatusFailure;
        }

        int bindResult = -1;

        __try
        {
            bindResult = bind(fd, &sa, sa_size);
        }
        __except (SocketExceptionFilter(GetExceptionCode()))
        {
        }

        if (bindResult == -1)
        {
            StoreLastError();
            return kWaitStatusFailure;
        }

        return kWaitStatusSuccess;
    }

    WaitStatus SocketImpl::Bind(uint32_t address, uint16_t port)
    {
        struct sockaddr sa = {0};
        socklen_t sa_size = 0;

        sockaddr_from_address(htonl(address), htons(port), &sa, &sa_size);

        SOCKET fd = (SOCKET)_fd;
        if (fd == -1)
        {
            Error::SetLastError(il2cpp::os::kWSAeshutdown);
            return kWaitStatusFailure;
        }

        int bindResult = -1;

        __try
        {
            bindResult = bind(fd, &sa, sa_size);
        }
        __except (SocketExceptionFilter(GetExceptionCode()))
        {
        }

        if (bindResult == -1)
        {
            StoreLastError();
            return kWaitStatusFailure;
        }

        return kWaitStatusSuccess;
    }

    WaitStatus SocketImpl::Bind(uint8_t address[ipv6AddressSize], uint32_t scope, uint16_t port)
    {
        IL2CPP_VM_NOT_SUPPORTED(SocketImpl::Bind, "We don't have IPv6 support on Windows yet.");
        return kWaitStatusFailure;
    }

    WaitStatus SocketImpl::ConnectInternal(struct sockaddr *sa, int32_t sa_size)
    {
        SOCKET fd = (SOCKET)_fd;
        if (fd == -1)
        {
            Error::SetLastError(il2cpp::os::kWSAeshutdown);
            return kWaitStatusFailure;
        }

        __try
        {
            if (connect(fd, sa, (socklen_t)sa_size) != -1)
                return kWaitStatusSuccess;

            if (errno != EINTR)
            {
                StoreLastError();
                return kWaitStatusFailure;
            }

            struct pollfd fds = { 0 };

            fds.fd = fd;
            fds.events = POLLOUT;

            while (WSAPoll(&fds, 1, -1) == -1)
            {
                if (errno != EINTR)
                {
                    StoreLastError();
                    return kWaitStatusFailure;
                }
            }

            int32_t so_error = 0;
            socklen_t len = sizeof(so_error);

            if (getsockopt(fd, SOL_SOCKET, SO_ERROR, (char*)&so_error, &len) == -1)
            {
                StoreLastError();
                return kWaitStatusFailure;
            }

            if (so_error != 0)
            {
                StoreLastError(so_error);
                return kWaitStatusFailure;
            }
        }
        __except (SocketExceptionFilter(GetExceptionCode()))
        {
            SetLastError(kWSAeshutdown);
            return kWaitStatusFailure;
        }

        return kWaitStatusSuccess;
    }

    WaitStatus SocketImpl::Connect(const char *path)
    {
        // NOTE(gab): not supported on Windows.
        return kWaitStatusFailure;
    }

    WaitStatus SocketImpl::Connect(uint32_t address, uint16_t port)
    {
        struct sockaddr sa = {0};
        socklen_t sa_size = 0;

        sockaddr_from_address(htonl(address), htons(port), &sa, &sa_size);

        return ConnectInternal((struct sockaddr *)&sa, sa_size);
    }

    WaitStatus SocketImpl::Connect(uint8_t address[ipv6AddressSize], uint32_t scope, uint16_t port)
    {
        IL2CPP_VM_NOT_SUPPORTED(SocketImpl::Connect, "We don't have IPv6 support on Windows yet.");
        return kWaitStatusFailure;
    }

    WaitStatus SocketImpl::GetLocalEndPointInfo(EndPointInfo &info)
    {
        // Note: the size here could probably be smaller
        uint8_t buffer[END_POINT_MAX_PATH_LEN + 3] = {0};
        socklen_t address_len = sizeof(buffer);

        SOCKET fd = (SOCKET)_fd;
        if (fd == -1)
        {
            Error::SetLastError(il2cpp::os::kWSAeshutdown);
            return kWaitStatusFailure;
        }

        int getsocknameResult = -1;

        __try
        {
            getsocknameResult = getsockname(fd, (struct sockaddr *)buffer, &address_len);
        }
        __except (SocketExceptionFilter(GetExceptionCode()))
        {
            SetLastError(kWSAeshutdown);
            return kWaitStatusFailure;
        }

        if (getsocknameResult == -1)
        {
            StoreLastError();
            return kWaitStatusFailure;
        }

        if (!socketaddr_to_endpoint_info((struct sockaddr *)buffer, address_len, info))
        {
            _saved_error = kWSAeafnosupport;
            return kWaitStatusFailure;
        }

        return kWaitStatusSuccess;
    }

    WaitStatus SocketImpl::GetRemoteEndPointInfo(EndPointInfo &info)
    {
        // Note: the size here could probably be smaller
        uint8_t buffer[END_POINT_MAX_PATH_LEN + 3] = {0};
        socklen_t address_len = sizeof(buffer);

        SOCKET fd = (SOCKET)_fd;
        if (fd == -1)
        {
            Error::SetLastError(il2cpp::os::kWSAeshutdown);
            return kWaitStatusFailure;
        }

        int getpeernameResult = -1;

        __try
        {
            getpeernameResult = getpeername(fd, (struct sockaddr *)buffer, &address_len);
        }
        __except (SocketExceptionFilter(GetExceptionCode()))
        {
        }

        if (getpeernameResult == -1)
        {
            StoreLastError();
            return kWaitStatusFailure;
        }

        if (!socketaddr_to_endpoint_info((struct sockaddr *)buffer, address_len, info))
        {
            _saved_error = kWSAeafnosupport;
            return kWaitStatusFailure;
        }

        return kWaitStatusSuccess;
    }

    WaitStatus SocketImpl::Listen(int32_t backlog)
    {
        SOCKET fd = (SOCKET)_fd;
        if (fd == -1)
        {
            Error::SetLastError(il2cpp::os::kWSAeshutdown);
            return kWaitStatusFailure;
        }

        int listenResult = -1;

        __try
        {
            listenResult = listen(fd, backlog);
        }
        __except (SocketExceptionFilter(GetExceptionCode()))
        {
        }

        if (listenResult == -1)
        {
            StoreLastError();
            return kWaitStatusFailure;
        }

        return kWaitStatusSuccess;
    }

    WaitStatus SocketImpl::Shutdown(int32_t how)
    {
        SOCKET fd = (SOCKET)_fd;
        if (fd == -1)
        {
            Error::SetLastError(il2cpp::os::kWSAeshutdown);
            return kWaitStatusFailure;
        }

        int shutdownResult = -1;

        __try
        {
            shutdownResult = shutdown(fd, how);
        }
        __except (SocketExceptionFilter(GetExceptionCode()))
        {
        }

        if (shutdownResult == -1)
        {
            StoreLastError();
            return kWaitStatusFailure;
        }

        if (how == SD_RECEIVE || how == SD_BOTH)
            _still_readable = 0;

        return kWaitStatusSuccess;
    }

    static SOCKET AcceptProtected(SOCKET fd)
    {
        SOCKET new_fd;

        do
        {
            __try
            {
                new_fd = accept(fd, NULL, 0);
            }
            __except (SocketExceptionFilter(GetExceptionCode()))
            {
                new_fd = -1;
                break;
            }
        }
        while (new_fd == -1 && errno == EINTR);

        return new_fd;
    }

    WaitStatus SocketImpl::Accept(os::Socket **socket)
    {
        SocketDescriptor new_fd = 0;

        *socket = NULL;

        SOCKET fd = (SOCKET)_fd;
        if (fd == -1)
        {
            Error::SetLastError(il2cpp::os::kWSAeshutdown);
            return kWaitStatusFailure;
        }

        new_fd = AcceptProtected(fd);

        if (new_fd == -1)
        {
            StoreLastError();

            return kWaitStatusFailure;
        }

        *socket = new os::Socket(_thread_status_callback);

        const WaitStatus status = (*socket)->Create(new_fd, _domain, _type, _protocol);

        if (status != kWaitStatusSuccess)
        {
            delete *socket;
            *socket = NULL;
            return status;
        }

        return kWaitStatusSuccess;
    }

    WaitStatus SocketImpl::Disconnect(bool reuse)
    {
        GUID GuidDisconnectEx = WSAID_DISCONNECTEX;
        LPFN_DISCONNECTEX disconnectEx;
        DWORD copied;

        SOCKET fd = (SOCKET)_fd;
        if (fd == -1)
        {
            Error::SetLastError(il2cpp::os::kWSAeshutdown);
            return kWaitStatusFailure;
        }

        __try
        {
            int32_t ret = WSAIoctl(fd, SIO_GET_EXTENSION_FUNCTION_POINTER, &GuidDisconnectEx, sizeof(GuidDisconnectEx), &disconnectEx, sizeof(disconnectEx), &copied, 0, 0);

            if (ret == SOCKET_ERROR)
            {
                StoreLastError();
                return kWaitStatusFailure;
            }

            if (!disconnectEx(fd, NULL, 0, NULL))
            {
                StoreLastError();
                return kWaitStatusFailure;
            }
        }
        __except (SocketExceptionFilter(GetExceptionCode()))
        {
            Error::SetLastError(il2cpp::os::kWSAeshutdown);
            return kWaitStatusFailure;
        }

        return kWaitStatusSuccess;
    }

    WaitStatus SocketImpl::Receive(const uint8_t *data, int32_t count, os::SocketFlags flags, int32_t *len)
    {
        *len = 0;

        const int32_t c_flags = convert_socket_flags(flags);

        if (c_flags == -1)
        {
            _saved_error = kWSAeopnotsupp;
            return kWaitStatusFailure;
        }

        return ReceiveFromInternal(data, count, c_flags, len, NULL, 0);
    }

    WaitStatus SocketImpl::ReceiveFromInternal(const uint8_t *data, size_t count, int32_t flags, int32_t *len, struct sockaddr *from, int32_t *fromlen)
    {
        int32_t ret = 0;
        IL2CPP_ASSERT(count < static_cast<size_t>(std::numeric_limits<int>::max()));

        SOCKET fd = (SOCKET)_fd;
        if (fd == -1)
        {
            Error::SetLastError(il2cpp::os::kWSAeshutdown);
            return kWaitStatusFailure;
        }

        do
        {
            __try
            {
                ret = recvfrom(fd, (char*)data, static_cast<int>(count), flags, from, (socklen_t*)fromlen);
            }
            __except (SocketExceptionFilter(GetExceptionCode()))
            {
                ret = -1;
                break;
            }
        }
        while (ret == -1 && errno == EINTR);

        if (ret == 0 && count > 0)
        {
            // According to the Linux man page, recvfrom only
            // returns 0 when the socket has been shut down
            // cleanly.  Turn this into an EINTR to simulate win32
            // behaviour of returning EINTR when a socket is
            // closed while the recvfrom is blocking (we use a
            // shutdown() in socket_close() to trigger this.) See
            // bug 75705.

            // Distinguish between the socket being shut down at
            // the local or remote ends, and reads that request 0
            // bytes to be read

            // If this returns FALSE, it means the socket has been
            // closed locally.  If it returns TRUE, but
            // still_readable != 1 then shutdown
            // (SHUT_RD|SHUT_RDWR) has been called locally.

            if (_still_readable != 1)
            {
                ret = -1;
                errno = EINTR;
            }
        }

        if (ret == -1)
        {
            StoreLastError();
            return kWaitStatusFailure;
        }

        *len = ret;

        return kWaitStatusSuccess;
    }

    WaitStatus SocketImpl::Send(const uint8_t *data, int32_t count, os::SocketFlags flags, int32_t *len)
    {
        *len = 0;

        const int32_t c_flags = convert_socket_flags(flags);

        if (c_flags == -1)
        {
            _saved_error = kWSAeopnotsupp;
            return kWaitStatusFailure;
        }

        SOCKET fd = (SOCKET)_fd;
        if (fd == -1)
        {
            Error::SetLastError(il2cpp::os::kWSAeshutdown);
            return kWaitStatusFailure;
        }

        int32_t ret = 0;

        do
        {
            __try
            {
                ret = send(fd, (char*)data, count, flags);
            }
            __except (SocketExceptionFilter(GetExceptionCode()))
            {
                ret = -1;
                break;
            }
        }
        while (ret == -1 && errno == EINTR);

        if (ret == -1)
        {
            StoreLastError();
            return kWaitStatusFailure;
        }

        *len = ret;
        return kWaitStatusSuccess;
    }

    WaitStatus SocketImpl::SendArray(WSABuf *wsabufs, int32_t count, int32_t *sent, SocketFlags flags)
    {
        DWORD c_flags = convert_socket_flags(flags);

        if (c_flags == -1)
        {
            _saved_error = kWSAeopnotsupp;
            return kWaitStatusFailure;
        }

        SOCKET fd = (SOCKET)_fd;
        if (fd == -1)
        {
            Error::SetLastError(il2cpp::os::kWSAeshutdown);
            return kWaitStatusFailure;
        }

        DWORD bytes_sent;

        int32_t ret = -1;

        __try
        {
            ret = WSASend(fd, (WSABUF*)wsabufs, count, &bytes_sent, c_flags, NULL, NULL);
        }
        __except (SocketExceptionFilter(GetExceptionCode()))
        {
        }

        if (ret == SOCKET_ERROR)
        {
            *sent = 0;

            StoreLastError();

            return kWaitStatusFailure;
        }

        *sent = bytes_sent;

        return kWaitStatusSuccess;
    }

    WaitStatus SocketImpl::ReceiveArray(WSABuf *wsabufs, int32_t count, int32_t *len, SocketFlags flags)
    {
        DWORD recv;
        DWORD c_flags = convert_socket_flags(flags);

        if (c_flags == -1)
        {
            _saved_error = kWSAeopnotsupp;
            return kWaitStatusFailure;
        }

        SOCKET fd = (SOCKET)_fd;
        if (fd == -1)
        {
            Error::SetLastError(il2cpp::os::kWSAeshutdown);
            return kWaitStatusFailure;
        }

        int32_t ret = -1;

        __try
        {
            // NOTE(gab): this cast is possible as long as WSABuf is binary compatible with WSABUF
            ret = WSARecv(fd, (WSABUF*)wsabufs, count, &recv, &c_flags, NULL, NULL);
        }
        __except (SocketExceptionFilter(GetExceptionCode()))
        {
        }

        if (ret == SOCKET_ERROR)
        {
            *len = 0;

            StoreLastError();

            return kWaitStatusFailure;
        }

        *len = recv;

        return kWaitStatusSuccess;
    }

    WaitStatus SocketImpl::SendTo(uint32_t address, uint16_t port, const uint8_t *data, int32_t count, os::SocketFlags flags, int32_t *len)
    {
        *len = 0;

        struct sockaddr sa = {0};
        socklen_t sa_size = 0;

        sockaddr_from_address(htonl(address), htons(port), &sa, &sa_size);

        const int32_t c_flags = convert_socket_flags(flags);

        if (c_flags == -1)
        {
            _saved_error = kWSAeopnotsupp;
            return kWaitStatusFailure;
        }

        SOCKET fd = (SOCKET)_fd;
        if (fd == -1)
        {
            Error::SetLastError(il2cpp::os::kWSAeshutdown);
            return kWaitStatusFailure;
        }

        int32_t ret = 0;

        do
        {
            __try
            {
                ret = sendto(fd, (char*)data, count, c_flags, &sa, sa_size);
            }
            __except (SocketExceptionFilter(GetExceptionCode()))
            {
                ret = -1;
                break;
            }
        }
        while (ret == -1 && errno == EINTR);

        if (ret == -1)
        {
            StoreLastError();
            return kWaitStatusFailure;
        }

        *len = ret;

        return kWaitStatusSuccess;
    }

    WaitStatus SocketImpl::SendTo(const char *path, const uint8_t *data, int32_t count, os::SocketFlags flags, int32_t *len)
    {
        // NOTE(gab): not supported on Windows.
        return kWaitStatusFailure;
    }

    WaitStatus SocketImpl::SendTo(uint8_t address[ipv6AddressSize], uint32_t scope, uint16_t port, const uint8_t *data, int32_t count, os::SocketFlags flags, int32_t *len)
    {
        IL2CPP_VM_NOT_SUPPORTED(SocketImpl::SendTo, "We don't have IPv6 support on Windows yet.");
        return kWaitStatusFailure;
    }

    WaitStatus SocketImpl::RecvFrom(uint32_t address, uint16_t port, const uint8_t *data, int32_t count, os::SocketFlags flags, int32_t *len, os::EndPointInfo &ep)
    {
        *len = 0;

        struct sockaddr sa = {0};
        socklen_t sa_size = 0;

        sockaddr_from_address(htonl(address), htons(port), &sa, &sa_size);

        const int32_t c_flags = convert_socket_flags(flags);

        if (c_flags == -1)
        {
            _saved_error = kWSAeopnotsupp;
            return kWaitStatusFailure;
        }

        int32_t ret = 0;

        const WaitStatus status = ReceiveFromInternal(data, count, c_flags, len, &sa, (int32_t*)&sa_size);

        if (status != kWaitStatusSuccess)
        {
            ep.family = os::kAddressFamilyError;
            return status;
        }

        if (sa_size == 0)
            return kWaitStatusSuccess;

        if (!socketaddr_to_endpoint_info(&sa, sa_size, ep))
        {
            ep.family = os::kAddressFamilyError;
            _saved_error = kWSAeafnosupport;
            return kWaitStatusFailure;
        }

        return kWaitStatusSuccess;
    }

    WaitStatus SocketImpl::RecvFrom(const char *path, const uint8_t *data, int32_t count, os::SocketFlags flags, int32_t *len, os::EndPointInfo &ep)
    {
        // NOTE(gab): not supported on Windows.
        return kWaitStatusFailure;
    }

    WaitStatus SocketImpl::RecvFrom(uint8_t address[ipv6AddressSize], uint32_t scope, uint16_t port, const uint8_t *data, int32_t count, os::SocketFlags flags, int32_t *len, os::EndPointInfo &ep)
    {
        IL2CPP_VM_NOT_SUPPORTED(SocketImpl::RecvFrom, "We don't have IPv6 support on Windows yet.");
        return kWaitStatusFailure;
    }

    WaitStatus SocketImpl::Available(int32_t *amount)
    {
        *amount = 0;

        u_long a = 0;

        SOCKET fd = (SOCKET)_fd;
        if (fd == -1)
        {
            Error::SetLastError(il2cpp::os::kWSAeshutdown);
            return kWaitStatusFailure;
        }

        int ioctlsocketResult = -1;

        __try
        {
            ioctlsocketResult = ioctlsocket(fd, FIONREAD, &a);
        }
        __except (SocketExceptionFilter(GetExceptionCode()))
        {
        }

        if (ioctlsocketResult == -1)
        {
            StoreLastError();
            return kWaitStatusFailure;
        }

        *amount = a;

        return kWaitStatusSuccess;
    }

    WaitStatus SocketImpl::Ioctl(int32_t command, const uint8_t *in_data, int32_t in_len, uint8_t *out_data, int32_t out_len, int32_t *written)
    {
        IL2CPP_ASSERT(command != 0xC8000006 /* SIO_GET_EXTENSION_FUNCTION_POINTER */ && "SIO_GET_EXTENSION_FUNCTION_POINTER ioctl command not supported");

        SOCKET fd = (SOCKET)_fd;
        if (fd == -1)
        {
            Error::SetLastError(il2cpp::os::kWSAeshutdown);
            return kWaitStatusFailure;
        }

        DWORD len = 0;
        int32_t ret = -1;

        __try
        {
            ret = WSAIoctl(fd, command, (void*)in_data, in_len, out_data, out_len, &len, NULL, NULL);
        }
        __except (SocketExceptionFilter(GetExceptionCode()))
        {
        }

        if (ret == SOCKET_ERROR)
        {
            StoreLastError();

            return kWaitStatusFailure;
        }

        *written = len;

        return kWaitStatusSuccess;
    }

#define     SKIP_OPTION         -2
#define     INVALID_OPTION_NAME -1

    static int32_t level_and_name_to_system(SocketOptionLevel level, SocketOptionName name, int32_t *system_level, int32_t *system_name)
    {
        switch (level)
        {
            case kSocketOptionLevelSocket:
                *system_level = SOL_SOCKET;

                switch (name)
                {
                    // This is SO_LINGER, because the setsockopt
                    // internal call maps DontLinger to SO_LINGER
                    // with l_onoff=0
                    case kSocketOptionNameDontLinger:
                        *system_name = SO_LINGER;
                        break;

                    case kSocketOptionNameDebug:
                        *system_name = SO_DEBUG;
                        break;

                    // #ifdef SO_ACCEPTCONN
                    case kSocketOptionNameAcceptConnection:
                        *system_name = SO_ACCEPTCONN;
                        break;
                    // #endif
                    case kSocketOptionNameReuseAddress:
                        *system_name = SO_REUSEADDR;
                        break;

                    case kSocketOptionNameKeepAlive:
                        *system_name = SO_KEEPALIVE;
                        break;

                    case kSocketOptionNameDontRoute:
                        *system_name = SO_DONTROUTE;
                        break;

                    case kSocketOptionNameBroadcast:
                        *system_name = SO_BROADCAST;
                        break;

                    case kSocketOptionNameLinger:
                        *system_name = SO_LINGER;
                        break;

                    case kSocketOptionNameOutOfBandInline:
                        *system_name = SO_OOBINLINE;
                        break;

                    case kSocketOptionNameSendBuffer:
                        *system_name = SO_SNDBUF;
                        break;

                    case kSocketOptionNameReceiveBuffer:
                        *system_name = SO_RCVBUF;
                        break;

                    case kSocketOptionNameSendLowWater:
                        *system_name = SO_SNDLOWAT;
                        break;

                    case kSocketOptionNameReceiveLowWater:
                        *system_name = SO_RCVLOWAT;
                        break;

                    case kSocketOptionNameSendTimeout:
                        *system_name = SO_SNDTIMEO;
                        break;

                    case kSocketOptionNameReceiveTimeout:
                        *system_name = SO_RCVTIMEO;
                        break;

                    case kSocketOptionNameError:
                        *system_name = SO_ERROR;
                        break;

                    case kSocketOptionNameType:
                        *system_name = SO_TYPE;
                        break;

                    case kSocketOptionNameExclusiveAddressUse:
        #ifdef SO_EXCLUSIVEADDRUSE
                        *system_name = SO_EXCLUSIVEADDRUSE;
                        break;
        #elif SO_REUSEADDR
                        *system_name = SO_REUSEADDR;
                        break;
        #endif
                    case kSocketOptionNameUseLoopback:
        #ifdef SO_USELOOPBACK
                        *system_name = SO_USELOOPBACK;
                        break;
        #endif
                    case kSocketOptionNameMaxConnections:
        #ifdef SO_MAXCONN
                        *system_name = SO_MAXCONN;
                        break;
        #elif defined(SOMAXCONN)
                        *system_name = SOMAXCONN;
                        break;
        #endif
                    default:
                        return INVALID_OPTION_NAME;
                }
                break;

            case kSocketOptionLevelIP:
#ifdef SOL_IP
                *system_level = SOL_IP;
#else
                *system_level = IPPROTO_IP;
#endif

                switch (name)
                {
                    case kSocketOptionNameIPOptions:
                        *system_name = IP_OPTIONS;
                        break;

        #ifdef IP_HDRINCL
                    case kSocketOptionNameHeaderIncluded:
                        *system_name = IP_HDRINCL;
                        break;
        #endif
        #ifdef IP_TOS
                    case kSocketOptionNameTypeOfService:
                        *system_name = IP_TOS;
                        break;
        #endif
        #ifdef IP_TTL
                    case kSocketOptionNameIpTimeToLive:
                        *system_name = IP_TTL;
                        break;
        #endif
                    case kSocketOptionNameMulticastInterface:
                        *system_name = IP_MULTICAST_IF;
                        break;

                    case kSocketOptionNameMulticastTimeToLive:
                        *system_name = IP_MULTICAST_TTL;
                        break;

                    case kSocketOptionNameMulticastLoopback:
                        *system_name = IP_MULTICAST_LOOP;
                        break;

                    case kSocketOptionNameAddMembership:
                        *system_name = IP_ADD_MEMBERSHIP;
                        break;

                    case kSocketOptionNameDropMembership:
                        *system_name = IP_DROP_MEMBERSHIP;
                        break;

        #ifdef HAVE_IP_PKTINFO
                    case kSocketOptionNamePacketInformation:
                        *system_name = IP_PKTINFO;
                        break;
        #endif

                    case kSocketOptionNameDontFragment:
        #ifdef IP_DONTFRAGMENT
                        *system_name = IP_DONTFRAGMENT;
        #elif IP_MTU_DISCOVER
                        *system_name = IP_MTU_DISCOVER;
        #elif IP_DONTFRAG
                        *system_name = IP_DONTFRAG;
        #else
                        return SKIP_OPTION;
        #endif
                        break;

                    case kSocketOptionNameAddSourceMembership:
                    case kSocketOptionNameDropSourceMembership:
                    case kSocketOptionNameBlockSource:
                    case kSocketOptionNameUnblockSource:
                    // Can't figure out how to map these, so fall
                    // through
                    default:
                        return INVALID_OPTION_NAME;
                }
                break;

            case kSocketOptionLevelTcp:
#ifdef SOL_TCP
                *system_level = SOL_TCP;
#else
                *system_level = IPPROTO_TCP;
#endif

                switch (name)
                {
                    case kSocketOptionNameNoDelay:
                        *system_name = TCP_NODELAY;
                        break;
                    default:
                        return INVALID_OPTION_NAME;
                }
                break;

            case kSocketOptionLevelUdp:
            default:
                return INVALID_OPTION_NAME;
        }

        return 0;
    }

    WaitStatus SocketImpl::GetSocketOption(SocketOptionLevel level, SocketOptionName name, uint8_t *buffer, int32_t *length)
    {
        int32_t system_level = 0;
        int32_t system_name = 0;

        const int32_t o_res = level_and_name_to_system(level, name, &system_level, &system_name);

        if (o_res == SKIP_OPTION)
        {
            *((int32_t*)buffer) = 0;
            *length = sizeof(int32_t);

            return kWaitStatusSuccess;
        }

        if (o_res == INVALID_OPTION_NAME)
        {
            _saved_error = kWSAenoprotoopt;

            return kWaitStatusFailure;
        }

        SOCKET fd = (SOCKET)_fd;
        if (fd == -1)
        {
            Error::SetLastError(il2cpp::os::kWSAeshutdown);
            return kWaitStatusFailure;
        }

        uint8_t *tmp_val = buffer;

        int32_t ret = -1;

        __try
        {
            ret = getsockopt(fd, system_level, system_name, (char*)tmp_val, (socklen_t*)length);
        }
        __except (SocketExceptionFilter(GetExceptionCode()))
        {
        }

        if (ret == -1)
        {
            StoreLastError();

            return kWaitStatusFailure;
        }

        if (system_name == SO_ERROR)
        {
            if (*((int32_t*)buffer) != 0)
            {
                StoreLastError(*((int32_t*)buffer));
            }
            else
            {
                *((int32_t*)buffer) = _saved_error;
            }
        }

        return kWaitStatusSuccess;
    }

    WaitStatus SocketImpl::GetSocketOptionFull(SocketOptionLevel level, SocketOptionName name, int32_t *first, int32_t *second)
    {
        int32_t system_level = 0;
        int32_t system_name = 0;

#if !defined(SO_EXCLUSIVEADDRUSE) && defined(SO_REUSEADDR)
        if (level == kSocketOptionLevelSocket && name == kSocketOptionNameExclusiveAddressUse)
        {
            system_level = SOL_SOCKET;
            system_name = SO_REUSEADDR;
        }
        else
#endif
        {
            const int32_t o_res = level_and_name_to_system(level, name, &system_level, &system_name);

            if (o_res == SKIP_OPTION)
            {
                *first = 0;
                *second = 0;

                return kWaitStatusSuccess;
            }

            if (o_res == INVALID_OPTION_NAME)
            {
                _saved_error = kWSAenoprotoopt;

                return kWaitStatusFailure;
            }
        }

        SOCKET fd = (SOCKET)_fd;
        if (fd == -1)
        {
            Error::SetLastError(il2cpp::os::kWSAeshutdown);
            return kWaitStatusFailure;
        }

        int32_t ret = -1;

        switch (name)
        {
            case kSocketOptionNameLinger:
            {
                struct linger linger;
                socklen_t lingersize = sizeof(linger);

                __try
                {
                    ret = getsockopt(fd, system_level, system_name, (char*)&linger, &lingersize);

                    *first = linger.l_onoff;
                    *second = linger.l_linger;
                }
                __except (SocketExceptionFilter(GetExceptionCode()))
                {
                    ret = -1;
                }
            }
            break;

            case kSocketOptionNameDontLinger:
            {
                struct linger linger;
                socklen_t lingersize = sizeof(linger);

                __try
                {
                    ret = getsockopt(fd, system_level, system_name, (char*)&linger, &lingersize);

                    *first = !linger.l_onoff;
                }
                __except (SocketExceptionFilter(GetExceptionCode()))
                {
                    ret = -1;
                }
            }
            break;

            case kSocketOptionNameSendTimeout:
            case kSocketOptionNameReceiveTimeout:
            {
                socklen_t time_ms_size = sizeof(*first);

                __try
                {
                    ret = getsockopt(fd, system_level, system_name, (char*)first, &time_ms_size);
                }
                __except (SocketExceptionFilter(GetExceptionCode()))
                {
                    ret = -1;
                }
            }
            break;

            default:
            {
                socklen_t valsize = sizeof(*first);

                __try
                {
                    ret = getsockopt(fd, system_level, system_name, (char*)first, &valsize);
                }
                __except (SocketExceptionFilter(GetExceptionCode()))
                {
                    ret = -1;
                }
            }
            break;
        }

        if (ret == -1)
        {
            StoreLastError();

            return kWaitStatusFailure;
        }

#if !defined(SO_EXCLUSIVEADDRUSE) && defined(SO_REUSEADDR)
        if (level == kSocketOptionLevelSocket && name == kSocketOptionNameExclusiveAddressUse)
            *first = *first ? 0 : 1;
#endif

        return kWaitStatusSuccess;
    }

    WaitStatus SocketImpl::Poll(std::vector<PollRequest> &requests, int32_t count, int32_t timeout, int32_t *result, int32_t *error)
    {
        const size_t nfds = (size_t)count;
        fd_set rfds, wfds, efds;

        FD_ZERO(&rfds);
        FD_ZERO(&wfds);
        FD_ZERO(&efds);

        for (size_t i = 0; i < nfds; i++)
        {
            SOCKET fd = static_cast<SOCKET>(requests[i].fd);
            requests[i].revents = kPollFlagsNone;
            if (fd == -1)
                continue;

            if ((requests[i].events & kPollFlagsIn) != 0)
                FD_SET(fd, &rfds);

            if ((requests[i].events & kPollFlagsOut) != 0)
                FD_SET(fd, &wfds);

            FD_SET(fd, &efds);
        }

        timeval timevalTimeout;
        timeval* timeoutPtr = NULL;
        if (timeout != -1)
        {
            timevalTimeout.tv_sec = timeout / 1000;
            timevalTimeout.tv_usec = (timeout % 1000) * 1000;
            timeoutPtr = &timevalTimeout;
        }

        int32_t affected = -1;

        __try
        {
            affected = select(0, &rfds, &wfds, &efds, timeoutPtr);
        }
        __except (SocketExceptionFilter(GetExceptionCode()))
        {
        }

        if (affected == -1)
        {
            *error = WSAGetLastError();

            // Mono does this as well and the threadpool-ms-io-poll code depends on this behavior
            if (*error == WSAENOTSOCK)
            {
                *error = os::SocketError::kInvalidHandle;
            }

            return kWaitStatusFailure;
        }

        int32_t resultCount = 0;
        for (size_t i = 0; i < nfds && affected > 0; i++)
        {
            SOCKET fd = static_cast<SOCKET>(requests[i].fd);
            if (fd == -1)
                continue;

            if ((requests[i].events & kPollFlagsIn) != 0 && FD_ISSET(fd, &rfds))
            {
                requests[i].revents |= kPollFlagsIn;
                affected--;
            }

            if ((requests[i].events & kPollFlagsOut) != 0 && FD_ISSET(fd, &wfds))
            {
                requests[i].revents |= kPollFlagsOut;
                affected--;
            }

            if (FD_ISSET(fd, &efds))
            {
                requests[i].revents |= kPollFlagsErr;
                affected--;
            }

            if (requests[i].revents != kPollFlagsNone)
                resultCount++;
        }

        *result = resultCount;
        return kWaitStatusSuccess;
    }

    WaitStatus SocketImpl::Poll(std::vector<PollRequest>& requests, int32_t timeout, int32_t *result, int32_t *error)
    {
        return Poll(requests, (int32_t)requests.size(), timeout, result, error);
    }

    WaitStatus SocketImpl::Poll(PollRequest& request, int32_t timeout, int32_t *result, int32_t *error)
    {
        std::vector<PollRequest> requests;
        requests.push_back(request);
        return Poll(requests, 1, timeout, result, error);
    }

    WaitStatus SocketImpl::SetSocketOption(SocketOptionLevel level, SocketOptionName name, int32_t value)
    {
        int32_t system_level = 0;
        int32_t system_name = 0;

        const int32_t o_res = level_and_name_to_system(level, name, &system_level, &system_name);

        if (o_res == SKIP_OPTION)
        {
            return kWaitStatusSuccess;
        }

        if (o_res == INVALID_OPTION_NAME)
        {
            _saved_error = kWSAenoprotoopt;

            return kWaitStatusFailure;
        }

        struct linger linger;

        WaitStatus ret = kWaitStatusFailure;

        switch (name)
        {
            case kSocketOptionNameDontLinger:
                linger.l_onoff = !value;
                linger.l_linger = 0;
                ret = SetSocketOptionInternal(system_level, system_name, &linger, sizeof(linger));
                break;

            case kSocketOptionNameDontFragment:
#ifdef IP_PMTUDISC_DO
                // Fiddle with the value slightly if we're turning DF on
                if (value == 1)
                    value = IP_PMTUDISC_DO;
#endif
                ret = SetSocketOptionInternal(system_level, system_name, (char*)&value, sizeof(value));
                break;

            default:
                ret = SetSocketOptionInternal(system_level, system_name, (char*)&value, sizeof(value));
                break;
        }

        return ret;
    }

    WaitStatus SocketImpl::SetSocketOptionLinger(SocketOptionLevel level, SocketOptionName name, bool enabled, int32_t seconds)
    {
        int32_t system_level = 0;
        int32_t system_name = 0;

        const int32_t o_res = level_and_name_to_system(level, name, &system_level, &system_name);

        if (o_res == SKIP_OPTION)
        {
            return kWaitStatusSuccess;
        }

        if (o_res == INVALID_OPTION_NAME)
        {
            _saved_error = kWSAenoprotoopt;

            return kWaitStatusFailure;
        }

        struct linger linger;

        linger.l_onoff = enabled;
        linger.l_linger = seconds;

        return SetSocketOptionInternal(system_level, system_name, &linger, sizeof(linger));
    }

    WaitStatus SocketImpl::SetSocketOptionArray(SocketOptionLevel level, SocketOptionName name, const uint8_t *buffer, int32_t length)
    {
        int32_t system_level = 0;
        int32_t system_name = 0;

        const int32_t o_res = level_and_name_to_system(level, name, &system_level, &system_name);

        if (o_res == SKIP_OPTION)
        {
            return kWaitStatusSuccess;
        }

        if (o_res == INVALID_OPTION_NAME)
        {
            _saved_error = kWSAenoprotoopt;

            return kWaitStatusFailure;
        }

        struct linger linger;

        WaitStatus ret = kWaitStatusFailure;

        switch (name)
        {
            case kSocketOptionNameDontLinger:
                if (length == 1)
                {
                    linger.l_linger = 0;
                    linger.l_onoff = (*((char*)buffer)) ? 0 : 1;

                    ret = SetSocketOptionInternal(system_level, system_name, &linger, sizeof(linger));
                }
                else
                {
                    _saved_error = kWSAeinval;

                    return kWaitStatusFailure;
                }
                break;

            default:
                ret = SetSocketOptionInternal(system_level, system_name, buffer, length);
                break;
        }

        return ret;
    }

    WaitStatus SocketImpl::SetSocketOptionMembership(SocketOptionLevel level, SocketOptionName name, uint32_t group_address, uint32_t local_address)
    {
        int32_t system_level = 0;
        int32_t system_name = 0;

        const int32_t o_res = level_and_name_to_system(level, name, &system_level, &system_name);

        if (o_res == SKIP_OPTION)
        {
            return kWaitStatusSuccess;
        }

        if (o_res == INVALID_OPTION_NAME)
        {
            _saved_error = kWSAenoprotoopt;

            return kWaitStatusFailure;
        }

        IP_MREQ mreq = {{0}};

        mreq.imr_multiaddr.s_addr = group_address;
        mreq.imr_interface.s_addr = local_address;

        return SetSocketOptionInternal(system_level, system_name, &mreq, sizeof(mreq));

        return kWaitStatusFailure;
    }

#if IL2CPP_SUPPORT_IPV6
    WaitStatus SocketImpl::SetSocketOptionMembership(SocketOptionLevel level, SocketOptionName name, IPv6Address ipv6, uint64_t interfaceOffset)
    {
        IL2CPP_VM_NOT_SUPPORTED(SocketImpl::SetSocketOptionMembership, "We don't have IPv6 support on Windows yet.");
        return kWaitStatusFailure;
    }

#endif

    WaitStatus SocketImpl::SetSocketOptionInternal(int32_t level, int32_t name, const void *value, int32_t len)
    {
        const void *real_val = value;

        SOCKET fd = (SOCKET)_fd;
        if (fd == -1)
        {
            Error::SetLastError(il2cpp::os::kWSAeshutdown);
            return kWaitStatusFailure;
        }

        int32_t ret = -1;

        __try
        {
            ret = setsockopt(fd, level, name, (const char*)real_val, (socklen_t)len);
        }
        __except (SocketExceptionFilter(GetExceptionCode()))
        {
        }

        if (ret == -1)
        {
            StoreLastError();

            return kWaitStatusFailure;
        }

        return kWaitStatusSuccess;
    }

    int32_t WSAIoctlProtected(SOCKET s, DWORD dwIoControlCode, LPVOID lpvInBuffer, DWORD cbInBuffer, LPVOID lpvOutBuffer, DWORD cbOutBuffer, LPDWORD lpcbBytesReturned, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
    {
        __try
        {
            return WSAIoctl(s, dwIoControlCode, lpvInBuffer, cbInBuffer, lpvOutBuffer, cbOutBuffer, lpcbBytesReturned, lpOverlapped, lpCompletionRoutine);
        }
        __except (SocketExceptionFilter(GetExceptionCode()))
        {
        }

        return -1;
    }

    BOOL transmitFileProtected(LPFN_TRANSMITFILE transmitFile, SOCKET hSocket, HANDLE hFile, DWORD nNumberOfBytesToWrite, DWORD nNumberOfBytesPerSend, LPOVERLAPPED lpOverlapped, LPTRANSMIT_FILE_BUFFERS lpTransmitBuffers, DWORD dwReserved)
    {
        __try
        {
            return transmitFile(hSocket, hFile, nNumberOfBytesToWrite, nNumberOfBytesPerSend, lpOverlapped, lpTransmitBuffers, dwReserved);
        }
        __except (SocketExceptionFilter(GetExceptionCode()))
        {
        }

        return FALSE;
    }

    WaitStatus SocketImpl::SendFile(const char *filename, TransmitFileBuffers *buffers, TransmitFileOptions options)
    {
        if (filename == NULL)
            return kWaitStatusFailure;

        const UTF16String wideFilename = utils::StringUtils::Utf8ToUtf16(filename);
        HANDLE file = CreateFileW((LPCWSTR)wideFilename.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

        if (file == INVALID_HANDLE_VALUE)
        {
            StoreLastError();

            return kWaitStatusFailure;
        }

        GUID transmitFileGuid = WSAID_TRANSMITFILE;
        LPFN_TRANSMITFILE transmitFile;
        DWORD copied;

        SOCKET fd = (SOCKET)_fd;
        if (fd == -1)
        {
            Error::SetLastError(il2cpp::os::kWSAeshutdown);
            return kWaitStatusFailure;
        }

        int32_t ret = WSAIoctlProtected(fd, SIO_GET_EXTENSION_FUNCTION_POINTER, &transmitFileGuid, sizeof(transmitFileGuid), &transmitFile, sizeof(transmitFile), &copied, 0, 0);

        if (ret == SOCKET_ERROR)
        {
            StoreLastError();

            return kWaitStatusFailure;
        }

        if (!transmitFileProtected(transmitFile, fd, file, 0, 0, NULL, (TRANSMIT_FILE_BUFFERS*)&buffers, options))
        {
            StoreLastError();

            CloseHandle(file);
            return kWaitStatusFailure;
        }

        CloseHandle(file);

        return kWaitStatusSuccess;
    }
}
}
#endif
