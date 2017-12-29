#ifndef MACHIAVELLI_MACH_VIEW_STATEHANDLERS_SERVERRUNNING_H_INCLUDED
#define MACHIAVELLI_MACH_VIEW_STATEHANDLERS_SERVERRUNNING_H_INCLUDED

#include <mach/infra/clicommand.h>

#include "mach/view/serverclistate.h"
#include "mach/view/statehandlers/statehandlerbase.h"

namespace mach
{
namespace view
{
namespace statehandlers
{
class ServerRunning : public StateHandlerBase<ServerRunning, ServerCliState::ServerRunning>
{
  private:
    int handle;

  public:
    using StateHandlerBase::StateHandlerBase;

    void EnterState() override;
    void ExitStateFromBase() noexcept override;
    void RenderConsole() const override;
};
}
}
}

#endif // #ifndef MACHIAVELLI_MACH_VIEW_STATEHANDLERS_SERVERRUNNING_H_INCLUDED
