#ifndef MACHIAVELLI_MACH_APP_COMMANDTYPE_H_INCLUDED
#define MACHIAVELLI_MACH_APP_COMMANDTYPE_H_INCLUDED

namespace mach
{
namespace app
{
enum class CommandType
{
    JoinGame,
    Choose,
    ChooseCharacter,
    BuildBuilding,
    GetGold,
    GetCard,
    UseCharacterPower,
    EndTurn
}; // enum class CommandType
} // namespace app
} // namespace mach

#endif // #ifndef MACHIAVELLI_MACH_APP_COMMANDTYPE_H_INCLUDED
