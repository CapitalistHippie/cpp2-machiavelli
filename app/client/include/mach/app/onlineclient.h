#ifndef MACHIAVELLI_MACH_APP_ONLINECLIENT_H_INCLUDED
#define MACHIAVELLI_MACH_APP_ONLINECLIENT_H_INCLUDED

#include <memory>
#include <string>

#include <mach/infra/serializer.h>
#include <mach/infra/subject.h>
#include <mach/infra/tcpclient.h>

#include <mach/app/constants.h>

#include "mach/app/client.h"
#include "mach/app/events/clientconnectedevent.h"
#include "mach/app/events/event.h"
#include "mach/app/eventtype.h"
#include "mach/app/eventvisitor.h"
#include "mach/app/onlineclientconfiguration.h"

namespace mach
{
namespace app
{
namespace detail
{
class EventObserverNotifierVisitor : public EventVisitor
{
  private:
    infra::Subject& eventSubject;

  public:
    EventObserverNotifierVisitor(infra::Subject& eventSubject)
      : eventSubject(eventSubject)
    {
    }

    void Visit(const events::ClientConnectedEvent& evt) const override
    {
        eventSubject.NotifyObservers(evt);
    }
}; // class EventObserverNotifierVisitor
} // namespace detail

class OnlineClient : public Client
{
  private:
    infra::TcpClient tcpClient;
    infra::Serializer serializer;

    OnlineClientConfiguration configuration;

    detail::EventObserverNotifierVisitor eventObserverNotifierVisitor;

    void NotifyObservers(std::shared_ptr<events::Event> evt) const;

  public:
    OnlineClient();

    void Connect();
    void StartAsync() override;

    const OnlineClientConfiguration& GetConfiguration() const;
    void SetConfiguration(OnlineClientConfiguration configuration);
}; // class OnlineClient
} // namespace app
} // namespace mach

#endif // #ifndef MACHIAVELLI_MACH_APP_ONLINECLIENT_H_INCLUDED
