#include "mach/app/onlineclient.h"

using namespace mach::app;

void OnlineClient::NotifyObservers(std::shared_ptr<events::Event> evt) const
{
    evt->Visit(eventObserverNotifierVisitor);
}

void mach::app::OnlineClient::ReadEventAsync()
{
    serializer.DeserializeAsync<events::Event, EventType>(tcpClient, [&](std::shared_ptr<events::Event> evt) {
        if (!isRunning)
        {
            return;
        }

        ReadEventAsync();
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
