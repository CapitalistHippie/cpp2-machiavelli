#include "mach/app/onlineclient.h"

using namespace mach::app;

void OnlineClient::NotifyObservers(std::shared_ptr<events::Event> evt) const
{
    evt->Visit(eventObserverNotifierVisitor);
}

mach::app::OnlineClient::OnlineClient()
  : eventObserverNotifierVisitor(eventSubject)
{
}

void OnlineClient::Connect()
{
    tcpClient.Connect(configuration.hostname, configuration.port);
}

void OnlineClient::StartAsync()
{
    auto evt = serializer.Deserialize<events::Event, EventType>(tcpClient);

    NotifyObservers(evt);
}

const OnlineClientConfiguration& OnlineClient::GetConfiguration() const
{
    return configuration;
}

void OnlineClient::SetConfiguration(OnlineClientConfiguration configuration)
{
    this->configuration = configuration;
}
