#ifndef MACHIAVELLI_MACH_DOMAIN_EVENTS_SERVERDISCONNECTEDEVENT_H_INCLUDED
#define MACHIAVELLI_MACH_DOMAIN_EVENTS_SERVERDISCONNECTEDEVENT_H_INCLUDED

#include <sstream>

#include "mach/domain/events/eventbase.h"
#include "mach/domain/eventtype.h"
#include "mach/domain/models/clientinfo.h"

namespace mach
{
namespace domain
{
namespace events
{
class ServerDisconnectedEvent : public EventBase<ServerDisconnectedEvent, EventType::ServerDisconnected>
{
  public:
    void Serialize(std::ostream& dataStream) const override
    {
    }

    void Deserialize(std::istream& dataStream) override
    {
    }

    unsigned int GetDataLength() const override
    {
        std::stringstream data;
        Serialize(data);

        return data.str().length();
    }
}; // class ServerDisconnectedEvent
} // namespace events
} // namespace domain
} // namespace mach

#endif // MACHIAVELLI_MACH_DOMAIN_EVENTS_SERVERDISCONNECTEDEVENT_H_INCLUDED
