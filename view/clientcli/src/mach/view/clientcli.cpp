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

    commandParser.RegisterCommand<std::string, int>("test");

    auto stopCommandHandler = std::bind(&ClientCli::Stop, this);

    commandSubject.RegisterObserver<infra::CliCommand>(
      [](const infra::CliCommand& command) { return command.name == "quit"; }, stopCommandHandler);
    commandSubject.RegisterObserver<infra::CliCommand>(
      [](const infra::CliCommand& command) { return command.name == "exit"; }, stopCommandHandler);
    commandSubject.RegisterObserver<infra::CliCommand>(
      [](const infra::CliCommand& command) { return command.name == "stop"; }, stopCommandHandler);

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
