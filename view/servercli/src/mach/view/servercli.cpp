#include "mach/view/servercli.h"

#include <functional>

#include <mach/infra/functionalerrorcategory.h>

using namespace mach;
using namespace mach::view;

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

    isRunning = true;

    RenderConsole();

    do
    {
        try
        {
            auto command = commandParser.ParseCommand(inputStream);
            commandMediator.HandleCommand(command);

            RenderConsole();
        }
        catch (const std::system_error& e)
        {
            if (e.code() == infra::FunctionalError::CliCommandNotRegistered)
            {
                RenderConsole();

                outputStream << "Unavailable or unknown command. Please try something else!\n\n";
            }
            else if (e.code() == infra::FunctionalError::CliCommandInvalidParameterInputType)
            {
                RenderConsole();

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

void ServerCli::RenderConsole() const {}
