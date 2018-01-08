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
class NextRound;
class NextTurn;
class GameStarted;
class GameEnded;
} // namespace events

class EventVisitor
{
  public:
    virtual ~EventVisitor() noexcept = default;

    MACHIAVELLI_MACH_DOMAIN_EVENTVISITOR_DEFINE_VISIT_METHOD(events::ClientConnectedEvent);
    MACHIAVELLI_MACH_DOMAIN_EVENTVISITOR_DEFINE_VISIT_METHOD(events::NextRound);
    MACHIAVELLI_MACH_DOMAIN_EVENTVISITOR_DEFINE_VISIT_METHOD(events::NextTurn);
    MACHIAVELLI_MACH_DOMAIN_EVENTVISITOR_DEFINE_VISIT_METHOD(events::GameStarted);
    MACHIAVELLI_MACH_DOMAIN_EVENTVISITOR_DEFINE_VISIT_METHOD(events::GameEnded);
}; // class EventVisitor
} // namespace domain
} // namespace mach

#endif // #ifndef MACHIAVELLI_MACH_DOMAIN_EVENTVISITOR_H_INCLUDED
