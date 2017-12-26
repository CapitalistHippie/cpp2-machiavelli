#include "mach/app/server.h"

using namespace mach;
using namespace mach::app;

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
