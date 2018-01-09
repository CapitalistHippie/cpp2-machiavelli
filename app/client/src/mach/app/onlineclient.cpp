#include "mach/app/onlineclient.h"

#include <mach/app/commands/buildbuildingcommand.h>
#include <mach/app/commands/chooseCharacterCommand.h>
#include <mach/app/commands/choosecommand.h>
#include <mach/app/commands/endturncommand.h>
#include <mach/app/commands/getcardcommand.h>
#include <mach/app/commands/getgoldcommand.h>
#include <mach/app/commands/joingamecommand.h>
#include <mach/app/commands/usecharacterpowercommand.h>

using namespace mach;
using namespace mach::app;

void OnlineClient::NotifyObservers(std::shared_ptr<domain::events::Event> evt) const
{
    evt->Visit(eventObserverNotifierVisitor);
}

void OnlineClient::JoinGame() const
{
    commands::JoinGameCommand command;
    command.playerName = configuration.playerName;

    auto data = serializer.Serialize(command);

    tcpClient.Write(data);
}

void OnlineClient::ReadEventsAsync()
{
    serializer.DeserializeAsync<domain::events::Event, domain::EventType>(
      tcpClient, [&](std::error_code error, std::shared_ptr<domain::events::Event> evt) {
          if (!isRunning)
          {
              return;
          }

          if (error == infra::SocketError::Connreset)
          {
              domain::events::ServerDisconnectedEvent evt;
              evt.Visit(eventObserverNotifierVisitor);

              // If the server disconnected, we stop the client.
              Stop();

              return;
          }

          ReadEventsAsync();
          NotifyObservers(evt);
      });
}

mach::app::OnlineClient::OnlineClient(infra::ThreadPool& threadPool)
  : threadPool(&threadPool)
  , tcpClient(threadPool)
  , eventObserverNotifierVisitor(eventSubject)
  , isConnected(false)
  , isRunning(false)
{
}

void OnlineClient::Connect()
{
    if (isConnected)
    {
        // TODO: Throw.
    }

    tcpClient.Connect(configuration.hostname, configuration.port);

    isConnected = true;
}

void OnlineClient::StartAsync()
{
    if (isRunning)
    {
        return;
    }

    if (!isConnected)
    {
        // TODO: Throw.
    }

    isRunning = true;

    JoinGame();

    ReadEventsAsync();
}

void OnlineClient::Stop()
{
    isRunning = false;
    isConnected = false;

    tcpClient.Disconnect();
}

void mach::app::OnlineClient::SendGetGoldCommand()
{
    commands::GetGoldCommand command;

    auto data = serializer.Serialize(command);

    tcpClient.Write(data);
}

void mach::app::OnlineClient::SendGetCardCommand()
{
    commands::GetCardCommand command;

    auto data = serializer.Serialize(command);

    tcpClient.Write(data);
}

void mach::app::OnlineClient::SendBuildBuildingCommand(int value)
{
    commands::BuildBuildingCommand command;
    command.chosenNumber = value;

    auto data = serializer.Serialize(command);

    tcpClient.Write(data);
}

void mach::app::OnlineClient::SendUseCharacterPowerCommand()
{
    commands::UseCharacterPowerCommand command;

    auto data = serializer.Serialize(command);

    tcpClient.Write(data);
}

void mach::app::OnlineClient::SendEndTurnCommand()
{
    commands::EndTurnCommand command;

    auto data = serializer.Serialize(command);

    tcpClient.Write(data);
}

void mach::app::OnlineClient::SendChooseCommand(int value)
{
    commands::ChooseCommand command;
    command.chosenNumber = value;

    auto data = serializer.Serialize(command);

    tcpClient.Write(data);
}

void mach::app::OnlineClient::SendChooseCharacterCommand(int value)
{
    commands::ChooseCharacterCommand command;
    command.chosenNumber = value;

    auto data = serializer.Serialize(command);

    tcpClient.Write(data);
}

const OnlineClientConfiguration& OnlineClient::GetConfiguration() const
{
    return configuration;
}

void OnlineClient::SetConfiguration(OnlineClientConfiguration configuration)
{
    this->configuration = configuration;
}
