#ifndef MACHIAVELLI_MACH_APP_COMMANDS_COMMAND_H_INCLUDED
#define MACHIAVELLI_MACH_APP_COMMANDS_COMMAND_H_INCLUDED

#include <mach/infra/serializable.h>

#include "mach/app/commandtype.h"

namespace mach
{
namespace app
{
class CommandVisitor;

namespace commands
{
class Command : public infra::Serializable
{
  public:
    virtual ~Command() noexcept = default;

    virtual void Visit(const CommandVisitor& visitor) const = 0;
}; // class Command
} // namespace commands
} // namespace app
} // namespace mach

#endif // MACHIAVELLI_MACH_APP_COMMANDS_COMMAND_H_INCLUDED
