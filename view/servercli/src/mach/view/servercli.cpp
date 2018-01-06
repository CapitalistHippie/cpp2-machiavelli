#include "mach/view/servercli.h"

#include <functional>
#include <system_error>

#include <mach/infra/abstractfactory.h>
#include <mach/infra/functionalerrorcategory.h>
#include <mach/view/console.h>

#include "mach/view/technicalerror.h"
#include "mach/view/technicalerrorcategory.h"

using namespace mach;
using namespace mach::view;

ServerCli::ServerCli(app::Server& server, std::istream& inputStream, std::ostream& outputStream)
  : server(server)
  , inputStream(inputStream)
  , outputStream(outputStream)
  , isRunning(false)
  , shouldStop(false)
{
}

void ServerCli::Start()
{
    if (IsRunning())
    {
        return;
    }

    isRunning = true;

    commandParser.RegisterCommand("quit");
    commandParser.RegisterCommand("exit");
    commandParser.RegisterCommand("stop");

    auto stopCommandNotificationHandler = std::bind(&ServerCli::Stop, this);

    commandSubject.RegisterObserver<infra::CliCommand>(
      [](const infra::CliCommand& command) { return command.name == "quit"; }, stopCommandNotificationHandler);
    commandSubject.RegisterObserver<infra::CliCommand>(
      [](const infra::CliCommand& command) { return command.name == "exit"; }, stopCommandNotificationHandler);
    commandSubject.RegisterObserver<infra::CliCommand>(
      [](const infra::CliCommand& command) { return command.name == "stop"; }, stopCommandNotificationHandler);

    // Set the initial state.
    SetState(ServerCliState::ConfigureServer);

    Render();

    do
    {
        try
        {
            auto command = commandParser.ParseCommand(inputStream);
            commandSubject.NotifyObservers(command);

            Render();
        }
        catch (const std::system_error& e)
        {
            if (e.code() == infra::FunctionalError::CliCommandNotRegistered)
            {
                Render();

                outputStream << "Unavailable or unknown command. Please try something else!\n\n";
            }
            else if (e.code() == infra::FunctionalError::CliCommandInvalidParameterInputType)
            {
                Render();

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

void ServerCli::Stop()
{
    if (!IsRunning())
    {
        return;
    }

    shouldStop = true;
}

bool ServerCli::IsRunning() const
{
    return isRunning;
}

void ServerCli::Render() const
{
    if (stateHandler == nullptr)
    {
        throw std::system_error(std::make_error_code(TechnicalError::NoStateHandler));
    }

    ClearConsole();

    outputStream << "Welcome to Machiavelli server!\n"
                 << "Enter exit, quit or stop to exit the application.\n\n";

    stateHandler->RenderConsole();

    outputStream << '\n';
}

void ServerCli::SetState(ServerCliState state)
{
    if (stateHandler != nullptr)
    {
        stateHandler->ExitState();
    }

    auto& factory = infra::AbstractFactory<statehandlers::StateHandler, ServerCliState>::GetInstance();

    stateHandler = factory.Construct(state, *this, server, commandParser, commandSubject, outputStream);

    stateHandler->EnterState();
}
