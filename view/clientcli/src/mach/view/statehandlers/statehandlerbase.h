#ifndef MACHIAVELLI_MACH_VIEW_STATEHANDLERS_BASESTATEHANDLER_H_INCLUDED
#define MACHIAVELLI_MACH_VIEW_STATEHANDLERS_BASESTATEHANDLER_H_INCLUDED

#include <istream>
#include <ostream>
#include <string>
#include <vector>

#include <mach/app/onlineclient.h>
#include <mach/infra/abstractfactory.h>
#include <mach/infra/clicommand.h>
#include <mach/infra/clicommandparser.h>
#include <mach/infra/subject.h>

#include "mach/view/clientcli.h"
#include "mach/view/clientclistate.h"
#include "mach/view/statehandlers/statehandler.h"

namespace mach
{
namespace view
{
namespace statehandlers
{
template<typename TDerived, ClientCliState state>
class StateHandlerBase : public StateHandler
{
  private:
    static const infra::AbstractFactory<StateHandler, ClientCliState>::ProductRegistrar<TDerived,
                                                                                        ClientCli&,
                                                                                        app::OnlineClient&,
                                                                                        infra::CliCommandParser&,
                                                                                        infra::Subject&,
                                                                                        std::ostream&>
      registrar;

    std::vector<std::string> commandNamesToUnregister;
    std::vector<infra::Subject::ObserverHandle> commandObserverHandlesToUnregister;
    std::vector<infra::Subject::ObserverHandle> clientObserverHandlesToUnregister;

  protected:
    ClientCli& context;
    app::OnlineClient& client;
    infra::CliCommandParser& commandParser;
    infra::Subject& commandSubject;
    std::ostream& outputStream;

  public:
    StateHandlerBase(ClientCli& context,
                     app::OnlineClient& client,
                     infra::CliCommandParser& commandParser,
                     infra::Subject& commandSubject,
                     std::ostream& outputStream)
      : context(context)
      , client(client)
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

        for (const auto& clientObserverHandle : clientObserverHandlesToUnregister)
        {
            client.eventSubject.UnregisterObserver(clientObserverHandle);
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

    template<typename TObservable, typename TObserver>
    void RegisterClientObserver(TObserver observer)
    {
        auto clientObserverHandle = client.eventSubject.RegisterObserver<TObservable>(observer);

        clientObserverHandlesToUnregister.push_back(clientObserverHandle);
    }

    ClientCliState GetState() const noexcept override
    {
        registrar.Dummy();

        return state;
    }
};

// Static member StateHandlerBase::registrar initalization.
template<typename TProduct, ClientCliState state>
const infra::AbstractFactory<StateHandler, ClientCliState>::
  ProductRegistrar<TProduct, ClientCli&, app::OnlineClient&, infra::CliCommandParser&, infra::Subject&, std::ostream&>
    StateHandlerBase<TProduct, state>::registrar(state);
}
}
}

#endif // MACHIAVELLI_MACH_VIEW_STATEHANDLERS_BASESTATEHANDLER_H_INCLUDED
