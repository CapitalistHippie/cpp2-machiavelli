#include "mach/app/server.h"

using namespace mach::app;

void Server::Start()
{
    tcpServer.StartListening(configuration.port);
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
