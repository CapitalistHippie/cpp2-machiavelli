#include "mach/view/clientcli.h"

#include <functional>
#include <system_error>

#include <mach/infra/abstractfactory.h>
#include <mach/infra/functionalerrorcategory.h>

#include "mach/view/technicalerror.h"
#include "mach/view/technicalerrorcategory.h"

using namespace mach;
using namespace mach::view;

ClientCli::ClientCli(app::OnlineClient& client, std::istream& inputStream, std::ostream& outputStream)
  : client(&client)
  , inputStream(inputStream)
  , outputStream(outputStream)
  , isRunning(false)
  , shouldStop(false)
{
}

void ClientCli::Start()
{
    if (IsRunning())
    {
        return;
    }

    isRunning = true;

    commandParser.RegisterCommand("quit");
    commandParser.RegisterCommand("exit");
    commandParser.RegisterCommand("stop");
    commandParser.RegisterCommand("hulp");

    commandParser.RegisterCommand<std::string, int>("test");

    auto stopCommandHandler = std::bind(&ClientCli::Stop, this);

    commandSubject.RegisterObserver<infra::CliCommand>(
      [](const infra::CliCommand& command) { return command.name == "quit"; }, stopCommandHandler);
    commandSubject.RegisterObserver<infra::CliCommand>(
      [](const infra::CliCommand& command) { return command.name == "exit"; }, stopCommandHandler);
    commandSubject.RegisterObserver<infra::CliCommand>(
      [](const infra::CliCommand& command) { return command.name == "stop"; }, stopCommandHandler);

    commandSubject.RegisterObserver<infra::CliCommand>(
      [](const infra::CliCommand& command) { return command.name == "hulp"; },
      [&](const infra::CliCommand& command) {
          outputStream << "-----Verloop van een speelbeurt-----\n"
                       << "Gold of Card: Neem 2 goudstukken of neem 2 kaarten en leg er 1 af.\n"
                       << "Build: Leg 1 bouwkaart neer en betaal de waarde.\n"
                       << "Power: Op elk moment te gebruiken.\n\n"
                       << "-----Powers-----\n"
                       << "1: Moordenaar: Vermoord een ander karakter.\n"
                       << "2: Dief: Steelt van een andere speler.\n"
                       << "3: Magiër: Ruilt bouwkaarten om.\n"
                       << "4: Koning: Begint de volgende beurt. Ontvangt van monumenten.\n"
                       << "5: Prediker: Is beschermd tegen de Condottiere. Ontvangt van kerkelijke gebouwen.\n"
                       << "6: Koopman: Ontvangt een extra goudstuk. Ontvangt van commerciële gebouwen.\n"
                       << "7: Bouwmeester: Trekt twee extra kaarten. Mag drie gebouwen bouwen.\n"
                       << "8: Condottiere: Vernietigt een gebouw. Ontvangt van alle militaire gebouwen.\n";
      });

    client->eventSubject.RegisterObserver<domain::events::ServerDisconnectedEvent>(
      [&](const domain::events::ServerDisconnectedEvent& evt) { outputStream << "Server disconnected.\n"; });

    SetState(ClientCliState::ConnectToServer);

    do
    {
        try
        {
            auto command = commandParser.ParseCommand(inputStream);
            commandSubject.NotifyObservers(command);
        }
        catch (const std::system_error& e)
        {
            if (e.code() == infra::FunctionalError::CliCommandNotRegistered)
            {
                outputStream << "Unavailable or unknown command. Please try something else!\n\n";
            }
            else if (e.code() == infra::FunctionalError::CliCommandInvalidParameterInputType)
            {
                outputStream << "Invalid parameter type. Please try something else!\n\n";
            }
            else
            {
                throw;
            }
        }
    } while (!shouldStop);

    commandParser.Clear();
    commandSubject.Clear();

    isRunning = false;
    shouldStop = false;
}

void ClientCli::Stop()
{
    if (!IsRunning())
    {
        return;
    }

    shouldStop = true;
}

bool ClientCli::IsRunning() const
{
    return isRunning;
}

void ClientCli::SetState(ClientCliState state)
{
    if (stateHandler != nullptr)
    {
        stateHandler->ExitState();
    }

    auto& factory = infra::AbstractFactory<statehandlers::StateHandler, ClientCliState>::GetInstance();

    stateHandler = factory.Construct(state, *this, *client, commandParser, commandSubject, outputStream);

    stateHandler->EnterState();
}
