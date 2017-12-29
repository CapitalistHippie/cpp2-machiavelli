#include "mach/view/statehandlers/serverrunning.h"

#include <functional>

#include <mach/app/events/clientconnectedevent.h>

using namespace mach;
using namespace mach::view::statehandlers;

void ServerRunning::EnterState()
{
    handle = server.eventSubject.RegisterObserver<app::events::ClientConnectedEvent>(
      [&](const app::events::ClientConnectedEvent& evt) {
          outputStream << "Client connected from '" << evt.client.GetSource() << "'.\n";
      });
}

void ServerRunning::ExitStateFromBase() noexcept
{
    server.eventSubject.UnregisterObserver(handle);
}

void ServerRunning::RenderConsole() const
{
}
