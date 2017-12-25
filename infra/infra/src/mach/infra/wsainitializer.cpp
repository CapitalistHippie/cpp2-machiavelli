#ifdef _WIN32

#include "mach/infra/wsainitializer.h"

#include <system_error>

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

using namespace mach::infra;

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

    ~WsaInitializer()
    {
        auto result = WSACleanup();
        if (result != NOERROR)
        {
            throw std::system_error(result, std::system_category());
        }
    }
} wsaInitializer;
}

void mach::infra::InitializeWsa() noexcept
{
}

#endif
