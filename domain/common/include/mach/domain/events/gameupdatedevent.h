#ifndef MACHIAVELLI_MACH_DOMAIN_EVENTS_GAMEUPDATED_H_INCLUDED
#define MACHIAVELLI_MACH_DOMAIN_EVENTS_GAMEUPDATED_H_INCLUDED

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
class GameUpdatedEvent : public EventBase<GameUpdatedEvent, EventType::GameUpdated>
{
  public:
    models::Game game;
    std::string message;

    void Serialize(std::ostream& dataStream) const override
    {
        dataStream << game << "," << message;
    }

    void Deserialize(std::istream& dataStream) override
    {
        dataStream >> game;
        dataStream.ignore();
        std::getline(dataStream, message);
    }

    unsigned int GetDataLength() const override
    {
        std::stringstream data;
        Serialize(data);

        return data.str().length();
    }
}; // class GameUpdatedEvent
} // namespace events
} // namespace domain
} // namespace mach

#endif
