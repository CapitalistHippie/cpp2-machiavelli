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
#include <mach/infra/subject.h>

#include "mach/view/servercli.h"
#include "mach/view/serverclistate.h"
#include "mach/view/statehandlers/statehandler.h"

namespace mach
{
namespace view
{
namespace statehandlers
{
template<typename TDerived, ServerCliState state>
class StateHandlerBase : public StateHandler
{
  private:
    static const infra::AbstractFactory<StateHandler, ServerCliState>::ProductRegistrar<TDerived,
                                                                                        ServerCli&,
                                                                                        std::shared_ptr<app::Server>,
                                                                                        infra::CliCommandParser&,
                                                                                        infra::Subject&,
                                                                                        std::ostream&>
      registrar;

    std::vector<std::string> commandNamesToUnregister;
    std::vector<infra::Subject::ObserverHandle> commandObserverHandlesToUnregister;

  protected:
    ServerCli& context;
    std::shared_ptr<app::Server> server;
    infra::CliCommandParser& commandParser;
    infra::Subject& commandSubject;
    std::ostream& outputStream;

  public:
    StateHandlerBase(ServerCli& context,
                     std::shared_ptr<app::Server> server,
                     infra::CliCommandParser& commandParser,
                     infra::Subject& commandSubject,
                     std::ostream& outputStream)
      : context(context)
      , server(server)
      , commandParser(commandParser)
      , commandSubject(commandSubject)
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

        for (const auto& commandObserverHandle : commandObserverHandlesToUnregister)
        {
            commandSubject.UnregisterPredicateObserver(commandObserverHandle);
        }

        ExitStateFromBase();
    }

    template<typename... TParameters>
    void RegisterCommand(std::string commandName)
    {
        commandParser.RegisterCommand<TParameters...>(commandName);

        commandNamesToUnregister.push_back(commandName);
    }

    template<typename TPredicate, typename TObserver>
    void RegisterCommandObserver(TPredicate predicate, TObserver observer)
    {
        auto commandObserverHandle = commandSubject.RegisterObserver<infra::CliCommand>(predicate, observer);

        commandObserverHandlesToUnregister.push_back(commandObserverHandle);
    }

    ServerCliState GetState() const noexcept override
    {
        registrar.Dummy();

        return state;
    }
};

// Static member StateHandlerBase::registrar initalization.
template<typename TProduct, ServerCliState state>
const infra::AbstractFactory<StateHandler, ServerCliState>::ProductRegistrar<TProduct,
                                                                             ServerCli&,
                                                                             std::shared_ptr<app::Server>,
                                                                             infra::CliCommandParser&,
                                                                             infra::Subject&,
                                                                             std::ostream&>
  StateHandlerBase<TProduct, state>::registrar(state);
}
}
}

#endif // MACHIAVELLI_MACH_VIEW_STATEHANDLERS_BASESTATEHANDLER_H_INCLUDED
