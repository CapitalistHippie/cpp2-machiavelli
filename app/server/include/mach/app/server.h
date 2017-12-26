#ifndef MACHIAVELLI_MACH_APP_SERVER_H_INCLUDED
#define MACHIAVELLI_MACH_APP_SERVER_H_INCLUDED

#include <vector>

#include <mach/infra/tcpserver.h>

#include "mach/app/serverclient.h"
#include "mach/app/serverconfiguration.h"

namespace mach
{
namespace app
{
class Server
{
  private:
    infra::TcpServer tcpServer;
    std::vector<app::ServerClient> clients;

    ServerConfiguration configuration;

  public:
    void Start();
    void Stop();
    bool IsRunning();

    const ServerConfiguration& GetConfiguration() const;
    void SetConfiguration(ServerConfiguration configuration);
};
}
}

#endif // MACHIAVELLI_MACH_APP_SERVER_H_INCLUDED
