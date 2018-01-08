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
class ClientConnectedEvent;
class NextRoundEvent;
class NextTurnEvent;
class GameStartedEvent;
class GameEndedEvent;
} // namespace events

class EventVisitor
{
  public:
    virtual ~EventVisitor() noexcept = default;

    MACHIAVELLI_MACH_DOMAIN_EVENTVISITOR_DEFINE_VISIT_METHOD(events::ClientConnectedEvent);
    MACHIAVELLI_MACH_DOMAIN_EVENTVISITOR_DEFINE_VISIT_METHOD(events::NextRoundEvent);
    MACHIAVELLI_MACH_DOMAIN_EVENTVISITOR_DEFINE_VISIT_METHOD(events::NextTurnEvent);
    MACHIAVELLI_MACH_DOMAIN_EVENTVISITOR_DEFINE_VISIT_METHOD(events::GameStartedEvent);
    MACHIAVELLI_MACH_DOMAIN_EVENTVISITOR_DEFINE_VISIT_METHOD(events::GameEndedEvent);
}; // class EventVisitor
} // namespace domain
} // namespace mach

#endif // #ifndef MACHIAVELLI_MACH_DOMAIN_EVENTVISITOR_H_INCLUDED
