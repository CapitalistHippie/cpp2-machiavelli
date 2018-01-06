#include "mach/view/statehandlers/waitforgametostart.h"

#include <mach/app/events/clientconnectedevent.h>
#include <mach/view/console.h>

using namespace mach;
using namespace mach::view::statehandlers;

void WaitForGameToStart::EnterState()
{
    ClearConsole();

    RegisterClientObserver<app::events::ClientConnectedEvent>([&](const app::events::ClientConnectedEvent& evt) {
        outputStream << "Client with id '" << evt.clientInfo.id << "' connected from '" << evt.clientInfo.source
                     << "'.\n";
    });

    outputStream << "Waiting for game to start...\n";
}

void WaitForGameToStart::ExitStateFromBase() noexcept
{
}
