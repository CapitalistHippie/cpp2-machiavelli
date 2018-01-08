#ifndef MACHIAVELLI_MACH_DOMAIN_EVENTS_CLIENTCONNECTEDEVENT_H_INCLUDED
#define MACHIAVELLI_MACH_DOMAIN_EVENTS_CLIENTCONNECTEDEVENT_H_INCLUDED

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
class ClientConnectedEvent : public EventBase<ClientConnectedEvent, EventType::ClientConnected>
{
  public:
    models::ClientInfo clientInfo;

    void Serialize(std::ostream& dataStream) const override
    {
        dataStream << clientInfo.id << ',' << clientInfo.source;
    }

    void Deserialize(std::istream& dataStream) override
    {
        dataStream >> clientInfo.id;
        dataStream.ignore();
        dataStream >> clientInfo.source;
    }

    unsigned int GetDataLength() const override
    {
        std::stringstream data;
        Serialize(data);

        return data.str().length();
    }
}; // class ClientConnectedEvent
} // namespace events
} // namespace domain
} // namespace mach

#endif // MACHIAVELLI_MACH_DOMAIN_EVENTS_CLIENTCONNECTEDEVENT_H_INCLUDED
