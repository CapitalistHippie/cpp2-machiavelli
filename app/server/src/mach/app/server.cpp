#include "mach/app/server.h"

using namespace mach::app;

Server::Server()
{
}

void Server::StartListening()
{
    tcpServer.StartListening(DefaultPort);
}

void Server::StopListening()
{
    tcpServer.StopListening();
}

bool mach::app::Server::IsListening()
{
    return tcpServer.IsListening();
}

ServerClient Server::AcceptClient()
{
    return ServerClient(tcpServer.AcceptClient());
}
