#include "mach/view/statehandlers/configureserver.h"

#include <functional>

using namespace mach;
using namespace mach::view::statehandlers;

void ConfigureServer::SetPlayerCountCommandNotificationHandler(const infra::CliCommand& command)
{
    auto playerCount = *std::static_pointer_cast<int>(command.parameters[0]);

    serverConfiguration.playerCount = playerCount;
}

void ConfigureServer::SetPortCommandNotificationHandler(const infra::CliCommand& command)
{
    auto port = *std::static_pointer_cast<unsigned int>(command.parameters[0]);

    serverConfiguration.port = port;
}

void ConfigureServer::StartCommandNotificationHandler()
{
    server.SetConfiguration(serverConfiguration);
    server.Start();

    context.SetState(ServerCliState::ServerRunning);
}

void ConfigureServer::EnterState()
{
    serverConfiguration = server.GetConfiguration();

    RegisterCommand<int>("setplayercount");
    RegisterCommand<unsigned int>("setport");
    RegisterCommand("start");

    RegisterCommandObserver(
      [](const infra::CliCommand& command) { return command.name == "setplayercount"; },
      std::bind(&ConfigureServer::SetPlayerCountCommandNotificationHandler, this, std::placeholders::_1));
    RegisterCommandObserver(
      [](const infra::CliCommand& command) { return command.name == "setport"; },
      std::bind(&ConfigureServer::SetPortCommandNotificationHandler, this, std::placeholders::_1));
    RegisterCommandObserver([](const infra::CliCommand& command) { return command.name == "start"; },
                            std::bind(&ConfigureServer::StartCommandNotificationHandler, this));
}

void ConfigureServer::ExitStateFromBase() noexcept
{
}

void ConfigureServer::RenderConsole() const
{
    outputStream << "Here you can configure the server.\n"
                 << "The server currently has the following options set:\n"
                 << "Required amount of players: " << serverConfiguration.playerCount << "\n"
                 << "Server port: " << serverConfiguration.port << "\n"
                 << "\nThe following commands are available:\n"
                 << "setplayercount <the amount of players> - This command sets the amount of players required before "
                    "the game begins.\n"
                 << "setport <port> - Sets the port the server will be listening on.\n"
                 << "start - Start the server.";
}
