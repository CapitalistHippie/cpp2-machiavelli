#ifndef MACHIAVELLI_MACH_INFRA_TCPSERVERCLIENT_H_INCLUDED
#define MACHIAVELLI_MACH_INFRA_TCPSERVERCLIENT_H_INCLUDED

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

#include "mach/infra/noncopyable.h"

namespace mach
{
namespace infra
{
class TcpServerClient : public Noncopyable
{
  private:
    SOCKET clientSocket;

  public:
    TcpServerClient(SOCKET socket);
};
}
}

#endif // MACHIAVELLI_MACH_INFRA_TCPSERVERCLIENT_H_INCLUDED
