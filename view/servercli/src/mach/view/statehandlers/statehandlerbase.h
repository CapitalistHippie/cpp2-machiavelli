#ifndef MACHIAVELLI_MACH_VIEW_STATEHANDLERS_BASESTATEHANDLER_H_INCLUDED
#define MACHIAVELLI_MACH_VIEW_STATEHANDLERS_BASESTATEHANDLER_H_INCLUDED

#include <istream>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

#include <mach/app/server.h>
#include <mach/infra/abstractfactory.h>
#include <mach/infra/clicommand.h>
#include <mach/infra/clicommandparser.h>
#include <mach/infra/commandmediator.h>

#include "mach/view/servercli.h"
#include "mach/view/serverstate.h"
#include "mach/view/statehandlers/statehandler.h"

namespace mach
{
namespace view
{
namespace statehandlers
{
template<typename TDerived, ServerState state>
class StateHandlerBase : public StateHandler
{
  private:
    static const infra::AbstractFactory<StateHandler, ServerState>::ProductRegistrar<TDerived,
                                                                                     ServerCli&,
                                                                                     std::shared_ptr<app::Server>,
                                                                                     infra::CliCommandParser&,
                                                                                     infra::CommandMediator&,
                                                                                     std::ostream&>
      registrar;

    std::vector<std::string> commandNamesToUnregister;
    std::vector<infra::CommandMediator::CommandHandlerHandle> commandHandlerHandlesToUnregister;

  protected:
    ServerCli& context;
    std::shared_ptr<app::Server> server;
    infra::CliCommandParser& commandParser;
    infra::CommandMediator& commandMediator;
    std::ostream& outputStream;

  public:
    StateHandlerBase(ServerCli& context,
                     std::shared_ptr<app::Server> server,
                     infra::CliCommandParser& commandParser,
                     infra::CommandMediator& commandMediator,
                     std::ostream& outputStream)
      : context(context)
      , server(server)
      , commandParser(commandParser)
      , commandMediator(commandMediator)
      , outputStream(outputStream)
    {
    }
    virtual ~StateHandlerBase() = default;

    virtual void ExitStateFromBase() noexcept = 0;

    void ExitState() noexcept override
    {
        for (const auto& commandName : commandNamesToUnregister)
        {
            commandParser.UnregisterCommand(commandName);
        }

        for (const auto& commandHandlerHandle : commandHandlerHandlesToUnregister)
        {
            commandMediator.UnregisterCommandHandler(commandHandlerHandle);
        }

        ExitStateFromBase();
    }

    template<typename... TParameters>
    void RegisterCommand(std::string commandName)
    {
        commandParser.RegisterCommand<TParameters...>(commandName);

        commandNamesToUnregister.push_back(commandName);
    }

    template<typename TPredicate, typename THandler>
    void RegisterCommandHandler(TPredicate predicate, THandler handler)
    {
        auto commandHandlerHandle = commandMediator.RegisterCommandHandler<infra::CliCommand>(predicate, handler);

        commandHandlerHandlesToUnregister.push_back(commandHandlerHandle);
    }

    ServerState GetState() const noexcept override
    {
        registrar.Dummy();

        return state;
    }
};

// Static member StateHandlerBase::registrar initalization.
template<typename TProduct, ServerState state>
const infra::AbstractFactory<StateHandler, ServerState>::ProductRegistrar<TProduct,
                                                                          ServerCli&,
                                                                          std::shared_ptr<app::Server>,
                                                                          infra::CliCommandParser&,
                                                                          infra::CommandMediator&,
                                                                          std::ostream&>
  StateHandlerBase<TProduct, state>::registrar(state);
}
}
}

#endif // MACHIAVELLI_MACH_VIEW_STATEHANDLERS_BASESTATEHANDLER_H_INCLUDED
