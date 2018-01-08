#include "mach/view/statehandlers/serverrunning.h"

#include <functional>

#include <mach/domain/events/clientconnectedevent.h>
#include <mach/domain/events/gameendedevent.h>
#include <mach/domain/events/gamestartedevent.h>

using namespace mach;
using namespace mach::view::statehandlers;

void ServerRunning::EnterState()
{
    RegisterServerObserver<domain::events::ClientConnectedEvent>([&](const domain::events::ClientConnectedEvent& evt) {
        outputStream << "Client with id '" << evt.clientInfo.id << "' connected from '" << evt.clientInfo.source
                     << "'.\n";
    });

    RegisterServerObserver<domain::events::GameStartedEvent>(
      [&](const domain::events::GameStartedEvent& evt) { outputStream << "Game started.\n"; });

    RegisterServerObserver<domain::events::GameEndedEvent>(
      [&](const domain::events::GameEndedEvent& evt) { outputStream << "Game ended.\n"; });
}

void ServerRunning::ExitStateFromBase() noexcept
{
}

void ServerRunning::RenderConsole() const
{
}
