#include "mach/view/clientcli.h"

#include <stdlib.h>

#include <functional>
#include <system_error>

#include <mach/infra/abstractfactory.h>
#include <mach/infra/functionalerrorcategory.h>

#include "mach/view/technicalerror.h"
#include "mach/view/technicalerrorcategory.h"

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

ClientCli::ClientCli(std::istream& inputStream, std::ostream& outputStream)
  : inputStream(inputStream)
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

    commandSubject.RegisterObserver<infra::CliCommand>(
      [](const infra::CliCommand& command) { return command.name == "test"; },
      [&](const infra::CliCommand& command) {
          auto stringParameter = *std::static_pointer_cast<std::string>(command.parameters[0]);
          auto intParameter = *std::static_pointer_cast<int>(command.parameters[1]);
          outputStream << "string parameter: " << stringParameter << "\nint parameter: " << intParameter << "\n\n";
      });

    // Connect to the server.
    multiplayerClient.Connect("localhost");

    SetState(ClientCliState::ServerNotStarted);

    Render();

    do
    {
        try
        {
            auto command = commandParser.ParseCommand(inputStream);
            commandSubject.NotfiyObservers(command);

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

void ClientCli::Render() const
{
    ClearConsole();

    outputStream << "Welcome to Machiavelli!\n"
                 << "Enter exit, quit or stop to exit the application.\n\n";

    outputStream << '\n';
}

void ClientCli::SetState(ClientCliState state)
{
}
