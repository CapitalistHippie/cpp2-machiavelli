#ifndef MACHIAVELLI_MACH_APP_COMMANDS_JOINGAMECOMMAND_H_INCLUDED
#define MACHIAVELLI_MACH_APP_COMMANDS_JOINGAMECOMMAND_H_INCLUDED

#include <sstream>
#include <string>

#include "mach/app/commands/commandbase.h"
#include "mach/app/commandtype.h"

namespace mach
{
namespace app
{
namespace commands
{
class JoinGameCommand : public CommandBase<JoinGameCommand, CommandType::JoinGame>
{
  public:
    std::string playerName;

    void Serialize(std::ostream& dataStream) const override
    {
        dataStream << playerName;
    }

    void Deserialize(std::istream& dataStream) override
    {
        dataStream >> playerName;
    }

    unsigned int GetDataLength() const override
    {
        return playerName.length();
    }
}; // class ClientConnectedEveJoinGameCommandnt
} // namespace commands
} // namespace app
} // namespace mach

#endif // MACHIAVELLI_MACH_APP_COMMANDS_JOINGAMECOMMAND_H_INCLUDED
