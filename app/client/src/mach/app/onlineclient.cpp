#include "mach/app/onlineclient.h"

#include <mach/app/commands/joingamecommand.h>

using namespace mach::app;

void OnlineClient::NotifyObservers(std::shared_ptr<events::Event> evt) const
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
    serializer.DeserializeAsync<events::Event, EventType>(tcpClient, [&](std::shared_ptr<events::Event> evt) {
        if (!isRunning)
        {
            return;
        }

        ReadEventsAsync();
        NotifyObservers(evt);
    });
}

OnlineClient::OnlineClient(infra::ThreadPool& threadPool)
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

    ReadEventAsync();
}

const OnlineClientConfiguration& OnlineClient::GetConfiguration() const
{
    return configuration;
}

void OnlineClient::SetConfiguration(OnlineClientConfiguration configuration)
{
    this->configuration = configuration;
}
