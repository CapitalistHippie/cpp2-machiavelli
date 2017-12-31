#ifndef MACHIAVELLI_MACH_APP_SERVER_H_INCLUDED
#define MACHIAVELLI_MACH_APP_SERVER_H_INCLUDED

#include <unordered_map>

#include <mach/infra/subject.h>
#include <mach/infra/tcpclient.h>
#include <mach/infra/tcpserver.h>
#include <mach/infra/threadpool.h>

#include <mach/app/events/clientconnectedevent.h>

#include "mach/app/serverclient.h"
#include "mach/app/serverconfiguration.h"

namespace mach
{
namespace app
{
class Server
{
  private:
    infra::ThreadPool& threadPool;

    infra::TcpServer tcpServer;
    std::unordered_map<ServerClient::Id, ServerClient> clients;

    ServerConfiguration configuration;

    bool isRunning;

    void AcceptClientAsync();
    void AcceptClientAsyncCallbackHandler(infra::TcpClient tcpClient);

  public:
    infra::Subject eventSubject;

    Server(infra::ThreadPool& threadPool);

    void StartAsync();
    void Stop();
    bool IsRunning();

    const ServerConfiguration& GetConfiguration() const;
    void SetConfiguration(ServerConfiguration configuration);
}; // class Server
} // namespace app
} // namespace mach

#endif // #ifndef MACHIAVELLI_MACH_APP_SERVER_H_INCLUDED
