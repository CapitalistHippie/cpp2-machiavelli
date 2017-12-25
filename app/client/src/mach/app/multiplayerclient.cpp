#include "mach/app/multiplayerclient.h"

using namespace mach::app;

void MultiplayerClient::Connect(const std::string& hostname)
{
    tcpClient.Connect(hostname, DefaultPort);
}
