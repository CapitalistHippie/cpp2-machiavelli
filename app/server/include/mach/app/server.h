#ifndef MACHIAVELLI_MACH_APP_SERVER_H_INCLUDED
#define MACHIAVELLI_MACH_APP_SERVER_H_INCLUDED

#include <system_error>
#include <unordered_map>
#include <vector>

#include <mach/domain/gamecontroller.h>

#include <mach/infra/serializer.h>
#include <mach/infra/socketerrorcategory.h>
#include <mach/infra/subject.h>
#include <mach/infra/tcpclient.h>
#include <mach/infra/tcpserver.h>
#include <mach/infra/threadpool.h>

#include <mach/app/commands/joingamecommand.h>
#include <mach/app/commandvisitor.h>
#include <mach/app/events/clientconnectedevent.h>

#include "mach/app/serverclient.h"
#include "mach/app/serverconfiguration.h"

namespace mach
{
namespace app
{
namespace detail
{
class CommandHandlerVisitor : public CommandVisitor
{
  private:
    domain::GameController* gameController;

  public:
    CommandHandlerVisitor(domain::GameController& gameController)
      : gameController(&gameController)
    {
    }

    void Visit(const commands::JoinGameCommand& command) const override
    {
        gameController->AddPlayer(command.playerName);
    }
}; // class CommandHandlerVisitor
} // namespace detail

class Server
{
  private:
    infra::ThreadPool* threadPool;

    infra::TcpServer tcpServer;
    std::unordered_map<ServerClient::Id, ServerClient> clients;

    infra::Serializer serializer;

    ServerConfiguration configuration;

    domain::GameController gameController;

    detail::CommandHandlerVisitor commandHandlerVisitor;

    bool isRunning;

    void AcceptClientAsync();
    void AcceptClientAsyncCallbackHandler(infra::TcpClient tcpClient);

    template<typename T>
    void NotifyObservers(const T& evt)
    {
        eventSubject.NotifyObservers(evt);

        auto data = serializer.Serialize(evt);

        std::vector<ServerClient::Id> clientsToRemove;

        for (auto& clientPair : clients)
        {
            auto& client = clientPair.second;

            try
            {
                client.tcpClient.Write(data);
            }
            catch (std::system_error& e)
            {
                if (e.code() == infra::SocketError::Connreset)
                {
                    clientsToRemove.push_back(client.id);
                }
            }
        }

        for (auto clientId : clientsToRemove)
        {
            // If the connection was forcibly closed on the other side we close the socket, remove the client and go on.
            clients.at(clientId).tcpClient.Disconnect();
            clients.erase(clientId);

            // TODO: Throw clientdisconnected event.
        }
    }

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
