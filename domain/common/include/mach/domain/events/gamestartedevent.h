#ifndef MACHIAVELLI_MACH_DOMAIN_EVENTS_GAMESTARTEDEVENT_H_INCLUDED
#define MACHIAVELLI_MACH_DOMAIN_EVENTS_GAMESTARTEDEVENT_H_INCLUDED

#include <sstream>

#include "mach/domain/events/eventbase.h"
#include "mach/domain/eventtype.h"
#include "mach\domain\models\game.h"
namespace mach
{
namespace domain
{
namespace events
{
class GameStartedEvent : public EventBase<GameStartedEvent, EventType::GameStarted>
{
  public:
    Game game;

    void Serialize(std::ostream& dataStream) const override
    {
        dataStream << game;
    }

    void Deserialize(std::istream& dataStream) override
    {
        dataStream >> game;
    }

    unsigned int GetDataLength() const override
    {
        std::stringstream data;
        data << game;

        return data.str().length();
    }
}; // class GameStartedEvent
} // namespace events
} // namespace domain
} // namespace mach

#endif
