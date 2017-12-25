#ifndef MACHIAVELLI_MACH_INFRA_TCPSERVER_H_INCLUDED
#define MACHIAVELLI_MACH_INFRA_TCPSERVER_H_INCLUDED

#include <cstdint>
#include <memory>
#include <string>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

#include "mach/infra/noncopyable.h"
#include "mach/infra/tcpserverclient.h"

namespace mach
{
namespace infra
{
class TcpServer : public Noncopyable
{
  public:
    typedef uint16_t Port;

  private:
    bool isListening;

    SOCKET listenSocket;

  public:
    TcpServer();

    void StartListening(Port port);
    void StopListening();
    bool IsListening();

    TcpServerClient AcceptClient();
};
}
}

#endif // MACHIAVELLI_MACH_INFRA_TCPSERVER_H_INCLUDED
