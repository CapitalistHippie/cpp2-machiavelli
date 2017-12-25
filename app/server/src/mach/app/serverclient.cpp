#include "mach/app/serverclient.h"

using namespace mach;
using namespace mach::app;

ServerClient::ServerClient(infra::TcpClient tcpClient)
  : tcpClient(std::move(tcpClient))
{
}

std::string ServerClient::GetSource() const
{
    return tcpClient.GetPeerAddress();
}
