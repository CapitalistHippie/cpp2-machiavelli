#ifndef MACHIAVELLI_MACH_VIEW_STATEHANDLERS_CONNECTTOSERVER_H_INCLUDED
#define MACHIAVELLI_MACH_VIEW_STATEHANDLERS_CONNECTTOSERVER_H_INCLUDED

#include <mach/infra/clicommand.h>

#include "mach/view/clientclistate.h"
#include "mach/view/statehandlers/statehandlerbase.h"

namespace mach
{
namespace view
{
namespace statehandlers
{
class ConnectToServer : public StateHandlerBase<ConnectToServer, ClientCliState::ConnectToServer>
{
  private:
    app::OnlineClientConfiguration clientConfiguration;

    void SetPlayerNameCommandHandler(const infra::CliCommand& command);
    void SetHostCommandHandler(const infra::CliCommand& command);
    void SetPortCommandHandler(const infra::CliCommand& command);
    void StartCommandHandler();

    void RenderConsole() const;

  public:
    using StateHandlerBase::StateHandlerBase;

    void EnterState() override;
    void ExitStateFromBase() noexcept override;
}; // class ConnectToServer
} // namespace statehandlers
} // namespace view
} // namespace mach

#endif // #ifndef MACHIAVELLI_MACH_VIEW_STATEHANDLERS_CONNECTTOSERVER_H_INCLUDED
