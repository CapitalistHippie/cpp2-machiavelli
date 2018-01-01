#ifndef MACHIAVELLI_MACH_APP_EVENTS_CLIENTCONNECTEDEVENT_H_INCLUDED
#define MACHIAVELLI_MACH_APP_EVENTS_CLIENTCONNECTEDEVENT_H_INCLUDED

#include <sstream>

#include "mach/app/events/eventbase.h"
#include "mach/app/eventtype.h"
#include "mach/app/models/clientinfo.h"

namespace mach
{
namespace app
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
        data << clientInfo.id << ',' << clientInfo.source;

        return data.str().length();
    }
}; // class ClientConnectedEvent
} // namespace events
} // namespace app
} // namespace mach

#endif // MACHIAVELLI_MACH_APP_EVENTS_CLIENTCONNECTEDEVENT_H_INCLUDED
