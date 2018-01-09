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
    std::vector<int> chosenNumbers;

    void Serialize(std::ostream& dataStream) const override
    {
        dataStream << chosenNumbers;
    }

    void Deserialize(std::istream& dataStream) override
    {
        dataStream >> chosenNumbers;
    }

    unsigned int GetDataLength() const override
    {
        std::stringstream data;
        Serialize(data);

        return data.str().length();
    }
}; // class ChooseCommand
} // namespace commands
} // namespace app
} // namespace mach

#endif
