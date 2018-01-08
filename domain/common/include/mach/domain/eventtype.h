#ifndef MACHIAVELLI_MACH_DOMAIN_EVENTTYPE_H_INCLUDED
#define MACHIAVELLI_MACH_DOMAIN_EVENTTYPE_H_INCLUDED

namespace mach
{
namespace domain
{
enum class EventType
{
    NextRound,
    NextTurn,
    GameStarted,
    GameEnded,
}; // enum class Event
} // namespace domain
} // namespace mach

#endif