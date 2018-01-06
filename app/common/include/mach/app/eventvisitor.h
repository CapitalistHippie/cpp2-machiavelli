#ifndef MACHIAVELLI_MACH_APP_EVENTVISITOR_H_INCLUDED
#define MACHIAVELLI_MACH_APP_EVENTVISITOR_H_INCLUDED

#define MACHIAVELLI_MACH_APP_EVENTVISITOR_DEFINE_VISIT_METHOD(eventType)                                               \
    virtual void Visit(const eventType& evt) const                                                                     \
    {                                                                                                                  \
    }

namespace mach
{
namespace app
{
namespace events
{
class ClientConnectedEvent;
} // namespace events

class EventVisitor
{
  public:
    virtual ~EventVisitor() noexcept = default;

    MACHIAVELLI_MACH_APP_EVENTVISITOR_DEFINE_VISIT_METHOD(events::ClientConnectedEvent);
}; // class EventVisitor
} // namespace app
} // namespace mach

#endif // #ifndef MACHIAVELLI_MACH_APP_EVENTVISITOR_H_INCLUDED
