#ifndef MACHIAVELLI_MACH_APP_SERVER_H_INCLUDED
#define MACHIAVELLI_MACH_APP_SERVER_H_INCLUDED

#include <mach/infra/tcpserver.h>

#include "mach/app/serverclient.h"

namespace mach
{
namespace app
{
class Server
{
  public:
    const static infra::TcpServer::Port DefaultPort = 4321;

  private:
    infra::TcpServer tcpServer;

  public:
    Server();

    void StartListening();
    void StopListening();
    bool IsListening();

    ServerClient AcceptClient();
};
}
}

#endif // MACHIAVELLI_MACH_APP_SERVER_H_INCLUDED
