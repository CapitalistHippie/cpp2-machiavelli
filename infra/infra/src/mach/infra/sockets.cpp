#include "mach/infra/sockets.h"

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
        if (result != NOERROR)
        {
            throw std::system_error(result, std::system_category());
        }
    }

    ~WsaInitializer() noexcept
    {
        auto result = WSACleanup();
        if (result != NOERROR)
        {
            // We shouldn't throw in a destructor.
            // throw std::system_error(result, std::system_category());
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
    if (result != NOERROR)
    {
        throw std::system_error(result, std::system_category());
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
    if (result != NOERROR)
    {
        throw std::system_error(GetLastSocketErrorCode(), std::system_category());
    }
#else // Assuming POSIX.
    int flags = fcntl(sock, F_GETFL, NULL);
    errno = 0;
    if (flags < 0 || fcntl(sock, F_SETFL, flags | O_NONBLOCK) < 0)
    {
        throw std::system_error(GetLastSocketErrorCode(), std::system_category());
    }
#endif
}

int mach::infra::CloseSocket(Socket socket)
{
    int status = 0;

#ifdef _WIN32
    if (closesocket(socket) == SOCKET_ERROR)
    {
        status = GetLastSocketErrorCode();
    }
#else // Assuming POSIX.
    status = close(socket);
#endif

    return status;
}

int mach::infra::GetLastSocketErrorCode()
{
#ifdef _WIN32
    return WSAGetLastError();
#else // Assuming POSIX.
    return errno;
#endif
}
