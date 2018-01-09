#include "mach/app/server.h"

#include <mach/app/commandtype.h>
#include <mach/domain/events/characterchosenevent.h>
#include <mach/domain/events/clientconnectedevent.h>
#include <mach/domain/events/gameendedevent.h>
#include <mach/domain/events/gamestartedevent.h>
#include <mach/domain/events/gameupdatedevent.h>
#include <mach/domain/events/illegalactionevent.h>
#include <mach/domain/events/nextroundevent.h>
#include <mach/domain/events/nextturnevent.h>

using namespace mach;
using namespace mach::app;
using namespace mach::app::events;

void Server::AcceptClientsAsync()
{
    tcpServer.AcceptClientAsync(std::bind(&Server::AcceptClientAsyncCallbackHandler, this, std::placeholders::_1));
}

void Server::AcceptClientAsyncCallbackHandler(infra::TcpClient tcpClient)
{
    // If the server has stopped it might be time for this loop to stop.
    if (!isRunning)
    {
        return;
    }

    if (clients.size() < configuration.playerCount)
    {
        // If we haven't reached the correct amount of players, accept another one.
        AcceptClientsAsync();
    }
    else
    {
        // Else we're done listening for new clients.
        tcpServer.StopListening();
    }

    ServerClient serverClient(std::move(tcpClient));
    auto serverClientId = serverClient.id;

    domain::events::ClientConnectedEvent evt;
    evt.clientInfo.id = serverClientId;
    evt.clientInfo.source = serverClient.GetSource();

    clients.emplace(serverClientId, std::move(serverClient));

    NotifyObservers(evt);

    ReadCommandsAsync(serverClientId);
}

void mach::app::Server::ReadCommandsAsync(ServerClient::Id clientId)
{
    // If the server has stopped it might be time for this loop to stop.
    if (!isRunning)
    {
        return;
    }

    // If the client has disconnected we stop the loop too.
    if (!IsClientConnected(clientId))
    {
        return;
    }

    const auto& client = clients.at(clientId);

    serializer.DeserializeAsync<commands::Command, CommandType>(
      client.tcpClient, [&, clientId](std::error_code error, std::shared_ptr<commands::Command> command) {
          if (!isRunning)
          {
              return;
          }

          ReadCommandsAsync(clientId);

          // Catch any commands that cause an exception and pass the error back to the client as an event.
          try
          {
              command->Visit(commandHandlerVisitor);
          }
          catch (std::exception& e)
          {
              domain::events::IllegalActionEvent evt;
              evt.message = e.what();

              NotifyClient(clientId, evt);
          }
      });
}

Server::Server(infra::ThreadPool& threadPool)
  : threadPool(&threadPool)
  , tcpServer(threadPool)
  , commandHandlerVisitor(gameController)
  , isRunning(false)
{
}

void Server::StartAsync()
{
    if (isRunning)
    {
        return;
    }

    tcpServer.StartListening(configuration.port);

    // Start accepting clients.
    AcceptClientsAsync();

    // Hook into all the domain events thrown from the game controller and pass them on to the observers.
    auto evtHandler = [&](const auto& evt) { NotifyObservers(evt); };

    gameController.eventSubject.RegisterObserver<domain::events::GameEndedEvent>(evtHandler);
    gameController.eventSubject.RegisterObserver<domain::events::GameStartedEvent>(evtHandler);
    gameController.eventSubject.RegisterObserver<domain::events::GameUpdatedEvent>(evtHandler);
    gameController.eventSubject.RegisterObserver<domain::events::IllegalActionEvent>(evtHandler);
    gameController.eventSubject.RegisterObserver<domain::events::CharacterChosenEvent>(evtHandler);
    gameController.eventSubject.RegisterObserver<domain::events::ClientConnectedEvent>(evtHandler);
    gameController.eventSubject.RegisterObserver<domain::events::NextRoundEvent>(evtHandler);
    gameController.eventSubject.RegisterObserver<domain::events::NextTurnEvent>(evtHandler);

    // We're in business now.
    isRunning = true;
}

void Server::Stop()
{
    if (!isRunning)
    {
        return;
    }

    isRunning = false;

    tcpServer.StopListening();

    gameController.eventSubject.Clear();
}

bool Server::IsRunning()
{
    return isRunning;
}

const ServerConfiguration& Server::GetConfiguration() const
{
    return configuration;
}

void Server::SetConfiguration(ServerConfiguration configuration)
{
    this->configuration = configuration;
}

bool mach::app::Server::IsClientConnected(ServerClient::Id clientId) const
{
    return clients.find(clientId) != clients.end();
}
