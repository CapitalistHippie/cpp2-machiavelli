#ifndef MACHIAVELLI_MACH_DOMAIN_EVENTS_GAMEENDEDEVENT_H_INCLUDED
#define MACHIAVELLI_MACH_DOMAIN_EVENTS_GAMEENDEDEVENT_H_INCLUDED

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
class GameEndedEvent : public EventBase<GameEndedEvent, EventType::GameEnded>
{
  public:
    Game game;
    std::string winner;

    void Serialize(std::ostream& dataStream) const override
    {
        dataStream << winner << "|";
        dataStream << game;
    }

    void Deserialize(std::istream& dataStream) override
    {
        dataStream >> winner;
        dataStream.ignore();
        dataStream >> game;
    }

    unsigned int GetDataLength() const override
    {
        std::stringstream data;
        data << winner << "|" << game;

        return data.str().length();
    }
}; // class GameStartedEvent
} // namespace events
} // namespace domain
} // namespace mach

#endif
