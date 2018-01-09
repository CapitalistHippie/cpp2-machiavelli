#ifndef MACHIAVELLI_MACH_APP_COMMANDS_GETGOLDCOMMAND_H_INCLUDED
#define MACHIAVELLI_MACH_APP_COMMANDS_GETGOLDCOMMAND_H_INCLUDED

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
class GetGoldCommand : public CommandBase<GetGoldCommand, CommandType::GetGold>
{
  public:
    void Serialize(std::ostream& dataStream) const override
    {
        dataStream << "***";
    }

    void Deserialize(std::istream& dataStream) override
    {
        dataStream.ignore();
        dataStream.ignore();
        dataStream.ignore();
    }

    unsigned int GetDataLength() const override
    {
        std::stringstream data;
        Serialize(data);

        return data.str().length();
    }
}; // class GetGoldCommand
} // namespace commands
} // namespace app
} // namespace mach

#endif
