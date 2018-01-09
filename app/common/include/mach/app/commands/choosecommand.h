#ifndef MACHIAVELLI_MACH_APP_COMMANDS_CHOOSECOMMAND_H_INCLUDED
#define MACHIAVELLI_MACH_APP_COMMANDS_CHOOSECOMMAND_H_INCLUDED

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
class ChooseCommand : public CommandBase<ChooseCommand, CommandType::Choose>
{
  public:
    int chosenNumber;

    void Serialize(std::ostream& dataStream) const override
    {
        dataStream << chosenNumber;
    }

    void Deserialize(std::istream& dataStream) override
    {
        dataStream >> chosenNumber;
    }

    unsigned int GetDataLength() const override
    {
        return 1;
    }
}; // class ChooseCommand
} // namespace commands
} // namespace app
} // namespace mach

#endif
