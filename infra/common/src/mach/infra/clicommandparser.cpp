#include "mach/infra/clicommandparser.h"

#include <sstream>
#include <system_error>

#include "mach/infra/functionalerror.h"
#include "mach/infra/functionalerrorcategory.h"

using namespace mach::infra;
using namespace mach::infra::detail;

void CliCommandParser::Clear()
{
    commandParameterParsers.clear();
}

void CliCommandParser::UnregisterCommand(const std::string& commandName)
{
    commandParameterParsers.erase(commandName);
}

CliCommand CliCommandParser::ParseCommand(std::istream& stream) const
{
    CliCommand command;

    // Parse the incoming command to a string stream buffer.
    std::string lineBuffer;
    std::getline(stream, lineBuffer);

    std::stringstream streamBuffer;
    streamBuffer << lineBuffer;

    // Parse the command.
    std::getline(streamBuffer, command.name, ' ');

    if (!IsCommandRegistered(command.name))
    {
        throw std::system_error(std::make_error_code(FunctionalError::CliCommandNotRegistered));
    }

    // Parse the command parameters.
    commandParameterParsers.at(command.name)->Parse(streamBuffer, command);

    return command;
}

bool CliCommandParser::IsCommandRegistered(const std::string& commandName) const
{
    return commandParameterParsers.find(commandName) != commandParameterParsers.end();
}
