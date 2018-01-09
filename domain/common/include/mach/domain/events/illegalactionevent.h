#ifndef MACHIAVELLI_MACH_DOMAIN_EVENTS_ILLEGALACTIONEVENT_H_INCLUDED
#define MACHIAVELLI_MACH_DOMAIN_EVENTS_ILLEGALACTIONEVENT_H_INCLUDED

#include <sstream>

#include "mach/domain/events/eventbase.h"
#include "mach/domain/eventtype.h"
#include "mach/domain/models/game.h"

namespace mach
{
namespace domain
{
namespace events
{
class IllegalActionEvent : public EventBase<IllegalActionEvent, EventType::IllegalAction>
{
  public:
    std::string message;

    void Serialize(std::ostream& dataStream) const override
    {
        dataStream << message;
    }

    void Deserialize(std::istream& dataStream) override
    {
        std::getline(dataStream, message);
    }

    unsigned int GetDataLength() const override
    {
        std::stringstream data;
        Serialize(data);

        return data.str().length();
    }
}; // class IllegalActionEvent
} // namespace events
} // namespace domain
} // namespace mach

#endif // #ifndef MACHIAVELLI_MACH_DOMAIN_EVENTS_CHARACTERCHOSENEVENT_H_INCLUDED
