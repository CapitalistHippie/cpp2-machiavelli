#ifndef MACHIAVELLI_MACH_DOMAIN_EVENTTYPE_H_INCLUDED
#define MACHIAVELLI_MACH_DOMAIN_EVENTTYPE_H_INCLUDED

namespace mach
{
namespace domain
{
enum class EventType
{
    CharacterChosen = 2000,
    ClientConnected,
    IllegalAction,
    NextRound,
    NextTurn,
    IntChoiceNecessary,
    CardChoiceNecessary,
    GameUpdated,
    GameStarted,
    GameEnded,
    ServerDisconnected
}; // enum class EventType
} // namespace domain
} // namespace mach

#endif
