#ifndef MACHIAVELLI_MACH_APP_COMMANDS_COMMANDBASE_H_INCLUDED
#define MACHIAVELLI_MACH_APP_COMMANDS_COMMANDBASE_H_INCLUDED

#include <string>

#include <mach/infra/abstractfactory.h>
#include <mach/infra/serializable.h>

#include "mach/app/commands/command.h"
#include "mach/app/commandtype.h"
#include "mach/app/commandvisitor.h"

namespace mach
{
namespace app
{
namespace commands
{
template<typename TDerived, CommandType identifier>
class CommandBase : public Command
{
  private:
    static const infra::AbstractFactory<Command, CommandType>::ProductRegistrar<TDerived> registrar;
    static const infra::AbstractFactory<infra::Serializable, CommandType>::ProductRegistrar<TDerived> registrar2;

  public:
    virtual ~CommandBase() noexcept = default;

    std::string GetIdentifier() const override
    {
        registrar.Dummy();
        registrar2.Dummy();

        return std::to_string(static_cast<unsigned int>(identifier));
    }

    void Visit(const CommandVisitor& visitor) const override
    {
        visitor.Visit(static_cast<const TDerived&>(*this));
    }
}; // class CommandBase

template<typename TProduct, CommandType identifier>
const infra::AbstractFactory<Command, CommandType>::ProductRegistrar<TProduct>
  CommandBase<TProduct, identifier>::registrar(identifier);

template<typename TProduct, CommandType identifier>
const infra::AbstractFactory<infra::Serializable, CommandType>::ProductRegistrar<TProduct>
  CommandBase<TProduct, identifier>::registrar2(identifier);
} // namespace commands
} // namespace app
} // namespace mach

#endif // MACHIAVELLI_MACH_APP_COMMANDS_COMMANDBASE_H_INCLUDED
