#ifndef MACHIAVELLI_MACH_APP_EVENTS_EVENT_H_INCLUDED
#define MACHIAVELLI_MACH_APP_EVENTS_EVENT_H_INCLUDED

#include <mach/infra/serializable.h>

#include "mach/app/eventtype.h"
#include "mach/app/models/clientinfo.h"

namespace mach
{
namespace app
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
} // namespace app
} // namespace mach

#endif // MACHIAVELLI_MACH_APP_EVENTS_EVENT_H_INCLUDED
