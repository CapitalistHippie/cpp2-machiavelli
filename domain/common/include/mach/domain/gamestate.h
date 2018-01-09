#ifndef MACHIAVELLI_MACH_DOMAIN_GAMESTATE_H_INCLUDED
#define MACHIAVELLI_MACH_DOMAIN_GAMESTATE_H_INCLUDED

namespace mach
{
namespace domain
{
enum class GameState
{
    Waiting,
    AwaitingPlayerChoice,
    ChoosingCharacters,
    Running,
    FinalRound,
    Ended
}; // enum class GameState
} // namespace domain
} // namespace mach

#endif // #ifndef MACHIAVELLI_MACH_DOMAIN_GAMESTATE_H_INCLUDED
