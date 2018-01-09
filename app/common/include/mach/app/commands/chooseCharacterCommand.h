#ifndef MACHIAVELLI_MACH_APP_COMMANDS_CHOOSECHARACTERCOMMAND_H_INCLUDED
#define MACHIAVELLI_MACH_APP_COMMANDS_CHOOSECHARACTERCOMMAND_H_INCLUDED

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
class ChooseCharacterCommand : public CommandBase<ChooseCharacterCommand, CommandType::ChooseCharacter>
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
        std::stringstream data;
        Serialize(data);

        return data.str().length();
    }
}; // class ChooseCharacterCommand
} // namespace commands
} // namespace app
} // namespace mach

#endif
