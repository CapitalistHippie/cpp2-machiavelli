#include "mach/view/servercli.h"

#include <stdlib.h>

#include <functional>

#include <mach/infra/abstractfactory.h>
#include <mach/infra/functionalerrorcategory.h>

using namespace mach;
using namespace mach::view;

inline void ClearConsole()
{
#ifdef _WIN32
    system("cls");
#else
    for (int i = 0; i < 100; ++i)
    {
        std::cout << '\n';
    }
#endif
}

ServerCli::ServerCli(std::shared_ptr<app::Server> server, std::istream& inputStream, std::ostream& outputStream)
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

    commandParser.RegisterCommand<std::string, int>("test");

    auto stopCommandHandler = std::bind(&ServerCli::Stop, this);

    commandMediator.RegisterCommandHandler<infra::CliCommand>(
      [](const infra::CliCommand& command) { return command.name == "quit"; }, stopCommandHandler);
    commandMediator.RegisterCommandHandler<infra::CliCommand>(
      [](const infra::CliCommand& command) { return command.name == "exit"; }, stopCommandHandler);
    commandMediator.RegisterCommandHandler<infra::CliCommand>(
      [](const infra::CliCommand& command) { return command.name == "stop"; }, stopCommandHandler);

    commandMediator.RegisterCommandHandler<infra::CliCommand>(
      [](const infra::CliCommand& command) { return command.name == "test"; },
      [&](const infra::CliCommand& command) {
          auto stringParameter = *std::static_pointer_cast<std::string>(command.parameters[0]);
          auto intParameter = *std::static_pointer_cast<int>(command.parameters[1]);
          outputStream << "string parameter: " << stringParameter << "\nint parameter: " << intParameter << "\n\n";
      });

    SetState(ServerState::ServerNotStarted);

    Render();

    do
    {
        try
        {
            auto command = commandParser.ParseCommand(inputStream);
            commandMediator.HandleCommand(command);

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
    commandMediator.Clear();

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
    ClearConsole();

    if (stateHandler == nullptr)
    {
        // TODO: Throw.
    }

    outputStream << "Welcome to Machiavelli server!\n"
                 << "Enter exit, quit or stop to exit the application.\n\n";

    stateHandler->RenderConsole();

    outputStream << '\n';
}

void ServerCli::SetState(ServerState state)
{
    if (stateHandler != nullptr)
    {
        stateHandler->ExitState();
    }

    auto& factory = infra::AbstractFactory<statehandlers::StateHandler, ServerState>::GetInstance();

    stateHandler = factory.Construct(state, *this, server, commandParser, commandMediator, outputStream);

    stateHandler->EnterState();
}
