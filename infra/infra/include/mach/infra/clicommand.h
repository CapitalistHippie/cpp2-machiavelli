#ifndef MACHIAVELLI_MACH_INFRA_CLICOMMAND_H_INCLUDED
#define MACHIAVELLI_MACH_INFRA_CLICOMMAND_H_INCLUDED

#include <memory>
#include <string>
#include <vector>

#include "mach/infra/command.h"
#include "mach/infra/functionalerror.h"
#include "mach/infra/functionalerrorcategory.h"

namespace mach
{
namespace infra
{
class CliCommand : public Command
{
  public:
    std::vector<std::shared_ptr<void>> parameters;
    std::string name;
}; // class CliCommand

namespace detail
{
template<typename... Parameters>
struct ParseCliCommandParametersImpl;

template<typename Parameter, typename... Parameters>
struct ParseCliCommandParametersImpl<Parameter, Parameters...>
{
    static void Parse(std::istream& stream, CliCommand& command)
    {
        Parameter parameter;

        stream >> parameter;
        if (stream.fail())
        {
            throw std::system_error(std::make_error_code(FunctionalError::CliCommandInvalidParameterInputType));
        }

        command.parameters.push_back(std::make_shared<Parameter>(std::move(parameter)));

        ParseCliCommandParametersImpl<Parameters...>::Parse(stream, command);
    }
};

template<typename... Parameters>
struct ParseCliCommandParametersImpl<std::string, Parameters...>
{
    static void Parse(std::istream& stream, CliCommand& command)
    {
        std::string parameter;

        std::getline(stream, parameter, ' ');
        if (stream.fail())
        {
            throw std::system_error(std::make_error_code(FunctionalError::CliCommandInvalidParameterInputType));
        }

        command.parameters.push_back(std::make_shared<std::string>(std::move(parameter)));

        ParseCliCommandParametersImpl<Parameters...>::Parse(stream, command);
    }
};

template<>
struct ParseCliCommandParametersImpl<std::string>
{
    static void Parse(std::istream& stream, CliCommand& command)
    {
        std::string parameter;

        std::getline(stream, parameter);
        if (stream.fail())
        {
            throw std::system_error(std::make_error_code(FunctionalError::CliCommandInvalidParameterInputType));
        }

        command.parameters.push_back(std::make_shared<std::string>(std::move(parameter)));
    }
};

template<>
struct ParseCliCommandParametersImpl<>
{
    static void Parse(std::istream& stream, CliCommand& command) {}
};

template<typename... Parameters>
void ParseCliCommandParameters(std::istream& stream, CliCommand& command)
{
    ParseCliCommandParametersImpl<Parameters...>::Parse(stream, command);
}
}
} // namespace infra
} // namespace mach

#endif // MACHIAVELLI_MACH_INFRA_CLICOMMAND_H_INCLUDED
