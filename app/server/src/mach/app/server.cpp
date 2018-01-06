#include "mach/app/server.h"

using namespace mach;
using namespace mach::app;
using namespace mach::app::events;

void Server::AcceptClientAsync()
{
    tcpServer.AcceptClientAsync(std::bind(&Server::AcceptClientAsyncCallbackHandler, this, std::placeholders::_1));
}

void Server::AcceptClientAsyncCallbackHandler(infra::TcpClient tcpClient)
{
    if (clients.size() != configuration.playerCount - 1)
    {
        // If we haven't reached the correct amount of players, accept another one.
        AcceptClientAsync();
    }
    else
    {
        // Else we're done listening for new clients.
        tcpServer.StopListening();

        // gameController.Start();
    }

    ServerClient serverClient(std::move(tcpClient));

    ClientConnectedEvent evt;
    evt.clientInfo.id = serverClient.id;
    evt.clientInfo.source = serverClient.GetSource();

    clients.emplace(serverClient.id, std::move(serverClient));

    NotifyObservers(evt);
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
    AcceptClientAsync();

    isRunning = true;
}

void Server::Stop()
{
    if (!isRunning)
    {
        return;
    }

    tcpServer.StopListening();

    isRunning = false;
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
