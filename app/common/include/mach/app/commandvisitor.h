#ifndef MACHIAVELLI_MACH_APP_COMMANDVISITOR_H_INCLUDED
#define MACHIAVELLI_MACH_APP_COMMANDVISITOR_H_INCLUDED

#define MACHIAVELLI_MACH_APP_COMMANDVISITOR_DEFINE_VISIT_METHOD(commandType)                                           \
    virtual void Visit(const commandType& evt) const                                                                   \
    {                                                                                                                  \
    }

namespace mach
{
namespace app
{
namespace commands
{
class JoinGameCommand;
} // namespace commands

class CommandVisitor
{
  public:
    virtual ~CommandVisitor() noexcept = default;

    MACHIAVELLI_MACH_APP_COMMANDVISITOR_DEFINE_VISIT_METHOD(commands::JoinGameCommand);
}; // class CommandVisitor
} // namespace app
} // namespace mach

#endif // #ifndef MACHIAVELLI_MACH_APP_COMMANDVISITOR_H_INCLUDED
