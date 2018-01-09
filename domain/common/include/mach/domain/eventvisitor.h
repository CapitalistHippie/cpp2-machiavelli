#ifndef MACHIAVELLI_MACH_DOMAIN_EVENTVISITOR_H_INCLUDED
#define MACHIAVELLI_MACH_DOMAIN_EVENTVISITOR_H_INCLUDED

#define MACHIAVELLI_MACH_DOMAIN_EVENTVISITOR_DEFINE_VISIT_METHOD(eventType)                                            \
    virtual void Visit(const eventType& evt) const = 0;

namespace mach
{
namespace domain
{
namespace events
{
class CharacterChosenEvent;
class ClientConnectedEvent;
class NextRoundEvent;
class NextTurnEvent;
class GameStartedEvent;
class GameEndedEvent;
class ServerDisconnectedEvent;
} // namespace events

class EventVisitor
{
  public:
    virtual ~EventVisitor() noexcept = default;

    MACHIAVELLI_MACH_DOMAIN_EVENTVISITOR_DEFINE_VISIT_METHOD(events::CharacterChosenEvent);
    MACHIAVELLI_MACH_DOMAIN_EVENTVISITOR_DEFINE_VISIT_METHOD(events::ClientConnectedEvent);
    MACHIAVELLI_MACH_DOMAIN_EVENTVISITOR_DEFINE_VISIT_METHOD(events::NextRoundEvent);
    MACHIAVELLI_MACH_DOMAIN_EVENTVISITOR_DEFINE_VISIT_METHOD(events::NextTurnEvent);
    MACHIAVELLI_MACH_DOMAIN_EVENTVISITOR_DEFINE_VISIT_METHOD(events::GameStartedEvent);
    MACHIAVELLI_MACH_DOMAIN_EVENTVISITOR_DEFINE_VISIT_METHOD(events::GameEndedEvent);
    MACHIAVELLI_MACH_DOMAIN_EVENTVISITOR_DEFINE_VISIT_METHOD(events::ServerDisconnectedEvent);
}; // class EventVisitor
} // namespace domain
} // namespace mach

#endif // #ifndef MACHIAVELLI_MACH_DOMAIN_EVENTVISITOR_H_INCLUDED
