#ifndef MACHIAVELLI_MACH_APP_ONLINECLIENT_H_INCLUDED
#define MACHIAVELLI_MACH_APP_ONLINECLIENT_H_INCLUDED

#include <memory>
#include <string>

#include <mach/infra/serializer.h>
#include <mach/infra/subject.h>
#include <mach/infra/tcpclient.h>
#include <mach/infra/threadpool.h>

#include <mach/domain/events/characterchosenevent.h>
#include <mach/domain/events/clientconnectedevent.h>
#include <mach/domain/events/event.h>
#include <mach/domain/events/gameendedevent.h>
#include <mach/domain/events/gamestartedevent.h>
#include <mach/domain/events/nextroundevent.h>
#include <mach/domain/events/nextturnevent.h>
#include <mach/domain/events/serverdisconnectedevent.h>
#include <mach/domain/eventvisitor.h>

#include "mach/app/client.h"
#include "mach/app/constants.h"
#include "mach/app/onlineclientconfiguration.h"

#define MACHIAVELLI_MACH_APP_ONLINECLIENT_DEFINE_VISIT_METHOD(eventType)                                               \
    void Visit(const eventType& evt) const override                                                                    \
    {                                                                                                                  \
        eventSubject->NotifyObservers(evt);                                                                            \
    }

namespace mach
{
namespace app
{
namespace detail
{
class EventObserverNotifierVisitor : public domain::EventVisitor
{
  private:
    infra::Subject* eventSubject;

  public:
    EventObserverNotifierVisitor(infra::Subject& eventSubject)
      : eventSubject(&eventSubject)
    {
    }

    MACHIAVELLI_MACH_APP_ONLINECLIENT_DEFINE_VISIT_METHOD(domain::events::CharacterChosenEvent);
    MACHIAVELLI_MACH_APP_ONLINECLIENT_DEFINE_VISIT_METHOD(domain::events::ClientConnectedEvent);
    MACHIAVELLI_MACH_APP_ONLINECLIENT_DEFINE_VISIT_METHOD(domain::events::NextRoundEvent);
    MACHIAVELLI_MACH_APP_ONLINECLIENT_DEFINE_VISIT_METHOD(domain::events::NextTurnEvent);
    MACHIAVELLI_MACH_APP_ONLINECLIENT_DEFINE_VISIT_METHOD(domain::events::GameStartedEvent);
    MACHIAVELLI_MACH_APP_ONLINECLIENT_DEFINE_VISIT_METHOD(domain::events::GameEndedEvent);
    MACHIAVELLI_MACH_APP_ONLINECLIENT_DEFINE_VISIT_METHOD(domain::events::ServerDisconnectedEvent);

}; // class EventObserverNotifierVisitor
} // namespace detail

class OnlineClient : public Client
{
  private:
    infra::ThreadPool* threadPool;
    infra::TcpClient tcpClient;
    infra::Serializer serializer;

    OnlineClientConfiguration configuration;

    detail::EventObserverNotifierVisitor eventObserverNotifierVisitor;

    bool isConnected;
    bool isRunning;

    void NotifyObservers(std::shared_ptr<domain::events::Event> evt) const;

    void JoinGame() const;
    void ReadEventsAsync();

  public:
    infra::Subject eventSubject;

    OnlineClient(infra::ThreadPool& threadPool);

    void Connect();
    void StartAsync() override;
    void Stop() override;

    const OnlineClientConfiguration& GetConfiguration() const;
    void SetConfiguration(OnlineClientConfiguration configuration);
}; // class OnlineClient
} // namespace app
} // namespace mach

#endif // #ifndef MACHIAVELLI_MACH_APP_ONLINECLIENT_H_INCLUDED
