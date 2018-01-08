#ifndef MACHIAVELLI_MACH_DOMAIN_EVENTTYPE_H_INCLUDED
#define MACHIAVELLI_MACH_DOMAIN_EVENTTYPE_H_INCLUDED

namespace mach
{
namespace domain
{
enum class EventType
{
    ClientConnected,
    NextRound,
    NextTurn,
    GameStarted,
    GameEnded,
    ServerDisconnected
}; // enum class EventType
} // namespace domain
} // namespace mach

#endif
