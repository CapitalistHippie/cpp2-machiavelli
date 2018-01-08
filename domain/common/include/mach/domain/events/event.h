#ifndef MACHIAVELLI_MACH_DOMAIN_EVENTS_EVENT_H_INCLUDED
#define MACHIAVELLI_MACH_DOMAIN_EVENTS_EVENT_H_INCLUDED

#include <mach/infra/serializable.h>

namespace mach
{
namespace domain
{
class EventVisitor;

namespace events
{
class Event : public infra::Serializable
{
  public:
    virtual ~Event() noexcept = default;

    virtual void Visit(const EventVisitor& visitor) const = 0;
}; // class Event
} // namespace events
} // namespace domain
} // namespace mach

#endif // #ifndef MACHIAVELLI_MACH_DOMAIN_EVENTS_EVENT_H_INCLUDED
