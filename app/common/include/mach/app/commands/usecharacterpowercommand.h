#ifndef MACHIAVELLI_MACH_APP_COMMANDS_USECHARACTERPOWER_H_INCLUDED
#define MACHIAVELLI_MACH_APP_COMMANDS_USECHARACTERPOWER_H_INCLUDED

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
class UseCharacterPowerCommand : public CommandBase<UseCharacterPowerCommand, CommandType::UseCharacterPower>
{
  public:
    void Serialize(std::ostream& dataStream) const override
    {
    }

    void Deserialize(std::istream& dataStream) override
    {
    }

    unsigned int GetDataLength() const override
    {
        std::stringstream data;
        Serialize(data);

        return data.str().length();
    }
}; // class UseCharacterPowerCommand
} // namespace commands
} // namespace app
} // namespace mach

#endif
