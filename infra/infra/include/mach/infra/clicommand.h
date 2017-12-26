#ifndef MACHIAVELLI_MACH_INFRA_CLICOMMAND_H_INCLUDED
#define MACHIAVELLI_MACH_INFRA_CLICOMMAND_H_INCLUDED

#include <memory>
#include <string>
#include <vector>

#include "mach/infra/functionalerror.h"
#include "mach/infra/functionalerrorcategory.h"
#include "mach/infra/thing.h"

namespace mach
{
namespace infra
{
class CliCommand : public Thing
{
  public:
    std::vector<std::shared_ptr<void>> parameters;
    std::string name;
}; // class CliCommand

namespace detail
{
template<typename... TParameters>
struct ParseCliCommandParametersImpl;

template<typename TParameter, typename... TParameters>
struct ParseCliCommandParametersImpl<TParameter, TParameters...>
{
    static void Parse(std::istream& stream, CliCommand& command)
    {
        TParameter parameter;

        stream >> parameter;
        if (stream.fail())
        {
            throw std::system_error(std::make_error_code(FunctionalError::CliCommandInvalidParameterInputType));
        }

        command.parameters.push_back(std::make_shared<TParameter>(std::move(parameter)));

        ParseCliCommandParametersImpl<TParameters...>::Parse(stream, command);
    }
};

template<typename... TParameters>
struct ParseCliCommandParametersImpl<std::string, TParameters...>
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

        ParseCliCommandParametersImpl<TParameters...>::Parse(stream, command);
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
    static void Parse(std::istream& stream, CliCommand& command)
    {
    }
};

template<typename... TParameters>
void ParseCliCommandParameters(std::istream& stream, CliCommand& command)
{
    ParseCliCommandParametersImpl<TParameters...>::Parse(stream, command);
}
}
} // namespace infra
} // namespace mach

#endif // MACHIAVELLI_MACH_INFRA_CLICOMMAND_H_INCLUDED
