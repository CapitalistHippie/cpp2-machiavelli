#include "mach/infra/socket.h"

#include "mach/infra/socketerrorcategory.h"

using namespace mach::infra;

#ifdef _WIN32
namespace
{
class WsaInitializer
{
  public:
    WsaInitializer()
    {
        WSADATA wsaData;
        auto result = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (result != 0)
        {
            throw std::system_error(ConvertSystemSocketErrorCode(result));
        }
    }

    ~WsaInitializer() noexcept
    {
        auto result = WSACleanup();
        if (result != 0)
        {
            // We shouldn't throw in a destructor.
            // throw std::system_error(ConvertSystemSocketErrorCode(result));
        }
    }
} wsaInitializer;
} // anonymous namespace
#endif // #ifdef _WIN32

bool mach::infra::IsInvalidSocket(Socket socket)
{
#ifdef _WIN32
    return socket == INVALID_SOCKET;
#else // Assuming POSIX.
    return socket < 0;
#endif
}

std::unique_ptr<addrinfo, void(__stdcall*)(addrinfo*)> mach::infra::GetAddrInfo(const char* hostname,
                                                                                const char* service,
                                                                                const addrinfo& hints)
{
    addrinfo* addrInfoResultBuffer = nullptr;
    std::unique_ptr<addrinfo, void(__stdcall*)(addrinfo*)> addrInfoResult(nullptr, &freeaddrinfo);

    auto result = getaddrinfo(hostname, service, &hints, &addrInfoResultBuffer);
    if (result != 0)
    {
        throw std::system_error(ConvertSystemSocketErrorCode(result));
    }

    addrInfoResult.reset(addrInfoResultBuffer);

    return addrInfoResult;
}

std::unique_ptr<addrinfo, void(__stdcall*)(addrinfo*)> mach::infra::GetAddrInfo(const char* hostname,
                                                                                Port port,
                                                                                const addrinfo& hints)
{
    return GetAddrInfo(hostname, std::to_string(port).c_str(), hints);
}

void mach::infra::SetSocketNonBlockingMode(Socket socket, bool enabled)
{
#ifdef _WIN32
    u_long blocking = static_cast<u_long>(enabled);
    int result = ioctlsocket(socket, FIONBIO, &blocking);
    if (result == SOCKET_ERROR)
    {
        throw std::system_error(GetLastSocketErrorCode());
    }
#else // Assuming POSIX.
    int flags = fcntl(sock, F_GETFL, NULL);
    errno = 0;
    if (flags < 0 || fcntl(sock, F_SETFL, flags | O_NONBLOCK) < 0)
    {
        throw std::system_error(GetLastSocketErrorCode());
    }
#endif
}

void mach::infra::CloseSocket(Socket socket)
{
#ifdef _WIN32
    if (closesocket(socket) == SOCKET_ERROR)
    {
        throw std::system_error(GetLastSocketErrorCode());
    }
#else // Assuming POSIX.
    if (close(socket) == -1)
    {
        throw std::system_error(GetLastSocketErrorCode());
    }
#endif
}

SocketError mach::infra::GetLastSocketErrorCode()
{
#ifdef _WIN32
    return ConvertSystemSocketErrorCode(WSAGetLastError());
#else // Assuming POSIX.
    return ConvertSystemSocketErrorCode(errno);
#endif
}

SocketError mach::infra::ConvertSystemSocketErrorCode(int errorCode)
{
#ifdef _WIN32
    switch (errorCode)
    {
        case 0:
            return SocketError::None;
        case WSAEWOULDBLOCK:
            return SocketError::Wouldblock;
        case WSAECONNRESET:
            return SocketError::Connreset;
        default:
            return SocketError::Other;
    }
#else // TODO: Assuming POSIX.
    switch (errno)
    {
        case 0:
            return SocketError::None;
        case EWOULDBLOCK:
            return SocketError::Wouldblock;
        case EAGAIN:
            return SocketError::Again;
        case ECONNRESET:
            return SocketError::Connreset;
        default:
            return SocketError::Other;
    }
#endif
}
