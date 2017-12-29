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
    }

    clients.push_back(std::move(tcpClient));

    // NOTE: This is dangerous as the clients container can change while the event is being handled and invalidate the
    // reference. Should probably place it before the AcceptClientAsync call or make a ClientInfo struct or something.
    ClientConnectedEvent evt(clients[clients.size() - 1]);

    eventSubject.NotifyObservers(evt);
}

Server::Server(infra::ThreadPool& threadPool)
  : tcpServer(threadPool)
  , threadPool(threadPool)
{
}

void Server::Start()
{
    tcpServer.StartListening(configuration.port);

    // Start accepting clients.
    AcceptClientAsync();
}

void Server::Stop()
{
    tcpServer.StopListening();
}

bool Server::IsRunning()
{
    return tcpServer.IsListening();
}

const ServerConfiguration& Server::GetConfiguration() const
{
    return configuration;
}

void Server::SetConfiguration(ServerConfiguration configuration)
{
    this->configuration = configuration;
}
