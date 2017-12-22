#ifndef MACHIAVELLI_MACH_VIEW_STATEHANDLERS_SERVERNOTSTARTED_H_INCLUDED
#define MACHIAVELLI_MACH_VIEW_STATEHANDLERS_SERVERNOTSTARTED_H_INCLUDED

#include <mach/infra/clicommand.h>

#include "mach/view/serverstate.h"
#include "mach/view/statehandlers/statehandlerbase.h"

namespace mach
{
namespace view
{
namespace statehandlers
{
class ServerNotStarted : public StateHandlerBase<ServerNotStarted, ServerState::ServerNotStarted>
{
  public:
    using StateHandlerBase::StateHandlerBase;

    void EnterState() override
    {
        RegisterCommand("start");
        RegisterCommandHandler([](const infra::CliCommand& command) { return command.name == "start"; },
                               [&](const infra::CliCommand& command) { outputStream << "Test handler thing\n"; });
    }

    void ExitStateFromBase() noexcept override
    {
    }

    void RenderConsole() const override
    {
        outputStream << "Top kek.\n";
    }
};
}
}
}

#endif // MACHIAVELLI_MACH_VIEW_STATEHANDLERS_SERVERNOTSTARTED_H_INCLUDED
