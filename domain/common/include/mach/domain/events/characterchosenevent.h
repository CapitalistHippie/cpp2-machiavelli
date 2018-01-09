#ifndef MACHIAVELLI_MACH_DOMAIN_EVENTS_CHARACTERCHOSENEVENT_H_INCLUDED
#define MACHIAVELLI_MACH_DOMAIN_EVENTS_CHARACTERCHOSENEVENT_H_INCLUDED

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
class CharacterChosenEvent : public EventBase<CharacterChosenEvent, EventType::CharacterChosen>
{
  public:
    models::Game game;

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
        Serialize(data);

        return data.str().length();
    }
}; // class CharacterChosenEvent
} // namespace events
} // namespace domain
} // namespace mach

#endif // #ifndef MACHIAVELLI_MACH_DOMAIN_EVENTS_CHARACTERCHOSENEVENT_H_INCLUDED
