#include "mach/view/statehandlers/connecttoserver.h"

#include <functional>

using namespace mach;
using namespace mach::view::statehandlers;

void ConnectToServer::EnterState()
{
    clientConfiguration = client.GetConfiguration();

    // Connect to the server.
    client.Connect();
    client.StartAsync();
    client.StartAsync();
}

void ConnectToServer::ExitStateFromBase() noexcept
{
}
