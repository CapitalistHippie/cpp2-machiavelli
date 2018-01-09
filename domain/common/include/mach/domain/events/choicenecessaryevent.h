#ifndef MACHIAVELLI_MACH_DOMAIN_EVENTS_CHOICENECESSARY_H_INCLUDED
#define MACHIAVELLI_MACH_DOMAIN_EVENTS_CHOICENECESSARY_H_INCLUDED

#include <sstream>

#include "mach/dal/models/buildingcard.h"
#include "mach/domain/events/eventbase.h"
#include "mach/domain/eventtype.h"

namespace mach
{
namespace domain
{
namespace events
{
class ChoiceNecessaryEvent : public EventBase<ChoiceNecessaryEvent, EventType::ChoiceNecessary>
{
  public:
    std::vector<dal::models::BuildingCard> choices;

    void Serialize(std::ostream& dataStream) const override
    {
        dataStream << choices;
    }

    void Deserialize(std::istream& dataStream) override
    {
        dataStream >> choices;
    }

    unsigned int GetDataLength() const override
    {
        std::stringstream data;
        Serialize(data);

        return data.str().length();
    }
}; // class ChoiceNecessaryEvent
} // namespace events
} // namespace domain
} // namespace mach

#endif // #ifndef MACHIAVELLI_MACH_DOMAIN_EVENTS_CHARACTERCHOSENEVENT_H_INCLUDED
