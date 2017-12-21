#ifndef MACHIAVELLI_MACH_INFRA_COMMAND_H_INCLUDED
#define MACHIAVELLI_MACH_INFRA_COMMAND_H_INCLUDED

namespace mach
{
namespace infra
{
class Command
{
  public:
    virtual ~Command() noexcept = default;
};
}
}

#endif // MACHIAVELLI_MACH_INFRA_COMMAND_H_INCLUDED
