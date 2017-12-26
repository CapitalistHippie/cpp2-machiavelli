#ifndef MACHIAVELLI_MACH_VIEW_STATEHANDLERS_CONFIGURESERVER_H_INCLUDED
#define MACHIAVELLI_MACH_VIEW_STATEHANDLERS_CONFIGURESERVER_H_INCLUDED

#include <mach/app/serverconfiguration.h>

#include <mach/infra/clicommand.h>

#include "mach/view/serverclistate.h"
#include "mach/view/statehandlers/statehandlerbase.h"

namespace mach
{
namespace view
{
namespace statehandlers
{
class ConfigureServer : public StateHandlerBase<ConfigureServer, ServerCliState::ConfigureServer>
{
  private:
    app::ServerConfiguration serverConfiguration;

    void SetPlayerCountCommandNotificationHandler(const infra::CliCommand& command);
    void SetPortCommandNotificationHandler(const infra::CliCommand& command);
    void StartCommandNotificationHandler();

  public:
    using StateHandlerBase::StateHandlerBase;

    void EnterState() override;
    void ExitStateFromBase() noexcept override;
    void RenderConsole() const override;
};
}
}
}

#endif // #ifndef MACHIAVELLI_MACH_VIEW_STATEHANDLERS_CONFIGURESERVER_H_INCLUDED
