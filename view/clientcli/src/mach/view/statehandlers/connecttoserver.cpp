#include "mach/view/statehandlers/connecttoserver.h"

#include <functional>

#include <mach/view/console.h>

using namespace mach;
using namespace mach::view::statehandlers;

void ConnectToServer::SetHostCommandHandler(const infra::CliCommand& command)
{
    auto host = *std::static_pointer_cast<std::string>(command.parameters[0]);

    clientConfiguration.hostname = host;

    RenderConsole();
}

void ConnectToServer::SetPortCommandHandler(const infra::CliCommand& command)
{
    auto port = *std::static_pointer_cast<infra::Port>(command.parameters[0]);

    clientConfiguration.port = port;

    RenderConsole();
}

void ConnectToServer::StartCommandHandler()
{
    client.SetConfiguration(clientConfiguration);

    client.Connect();
    client.StartAsync();

    context.SetState(ClientCliState::WaitForGameToStart);
}

void ConnectToServer::RenderConsole() const
{
    ClearConsole();

    outputStream << "Here you can configure the client.\n"
                 << "The client currently has the following options set:\n"
                 << "Server host: " << clientConfiguration.hostname << "\n"
                 << "Server port: " << clientConfiguration.port << "\n"
                 << "\nThe following commands are available:\n"
                 << "sethost <host> - Sets the server host to connect to.\n"
                 << "setport <port> - Sets the server port to connect to.\n"
                 << "start - Connect to the server and waits for the game to start.\n\n";
}

void ConnectToServer::EnterState()
{
    RegisterCommand<std::string>("sethost");
    RegisterCommand<infra::Port>("setport");
    RegisterCommand("start");

    RegisterCommandObserver([](const infra::CliCommand& command) { return command.name == "sethost"; },
                            std::bind(&ConnectToServer::SetHostCommandHandler, this, std::placeholders::_1));
    RegisterCommandObserver([](const infra::CliCommand& command) { return command.name == "setport"; },
                            std::bind(&ConnectToServer::SetPortCommandHandler, this, std::placeholders::_1));
    RegisterCommandObserver([](const infra::CliCommand& command) { return command.name == "start"; },
                            std::bind(&ConnectToServer::StartCommandHandler, this));

    clientConfiguration = client.GetConfiguration();

    RenderConsole();
}

void ConnectToServer::ExitStateFromBase() noexcept
{
}
