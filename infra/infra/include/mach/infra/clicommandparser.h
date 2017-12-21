#ifndef MACHIAVELLI_MACH_INFRA_CLICOMMANDPARSER_H_INCLUDED
#define MACHIAVELLI_MACH_INFRA_CLICOMMANDPARSER_H_INCLUDED

#include <istream>
#include <memory>
#include <string>
#include <unordered_map>

#include "mach/infra/clicommand.h"

namespace mach
{
namespace infra
{
namespace detail
{
class CliCommandParametersParser
{
  public:
    virtual ~CliCommandParametersParser() noexcept = default;

    virtual void Parse(std::istream& stream, CliCommand& command) const = 0;
}; // class CliCommandParametersParser

template<typename... TParameters>
class CliCommandParametersParserImpl : public CliCommandParametersParser
{
    void Parse(std::istream& stream, CliCommand& command) const override
    {
        ParseCliCommandParameters<TParameters...>(stream, command);
    }
}; // class CliCommandParametersParserImpl
} // namespace detail

class CliCommandParser
{
  private:
    std::unordered_map<std::string, std::shared_ptr<detail::CliCommandParametersParser>> commandParameterParsers;

  public:
    template<typename... TParameters>
    void RegisterCommand(std::string commandName)
    {
        commandParameterParsers.emplace(std::move(commandName),
                                        std::make_shared<detail::CliCommandParametersParserImpl<TParameters...>>());
    }

    void Clear();
    void UnregisterCommand(const std::string& commandName);
    CliCommand ParseCommand(std::istream& stream) const;
    bool IsCommandRegistered(const std::string& commandName) const;
}; // class CliCommandParser
} // namespace infra
} // namespace mach

#endif // MACHIAVELLI_MACH_INFRA_CLICOMMANDPARSER_H_INCLUDED
