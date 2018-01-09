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
class ChooseCommand;
class ChooseCharacterCommand;
class GetGoldCommand;
class GetCardCommand;
class UseCharacterPowerCommand;
class BuildBuildingCommand;
class EndTurnCommand;
} // namespace commands

class CommandVisitor
{
  public:
    virtual ~CommandVisitor() noexcept = default;

    MACHIAVELLI_MACH_APP_COMMANDVISITOR_DEFINE_VISIT_METHOD(commands::JoinGameCommand);
    MACHIAVELLI_MACH_APP_COMMANDVISITOR_DEFINE_VISIT_METHOD(commands::ChooseCommand);
    MACHIAVELLI_MACH_APP_COMMANDVISITOR_DEFINE_VISIT_METHOD(commands::ChooseCharacterCommand);
    MACHIAVELLI_MACH_APP_COMMANDVISITOR_DEFINE_VISIT_METHOD(commands::GetGoldCommand);
    MACHIAVELLI_MACH_APP_COMMANDVISITOR_DEFINE_VISIT_METHOD(commands::GetCardCommand);
    MACHIAVELLI_MACH_APP_COMMANDVISITOR_DEFINE_VISIT_METHOD(commands::UseCharacterPowerCommand);
    MACHIAVELLI_MACH_APP_COMMANDVISITOR_DEFINE_VISIT_METHOD(commands::BuildBuildingCommand);
    MACHIAVELLI_MACH_APP_COMMANDVISITOR_DEFINE_VISIT_METHOD(commands::EndTurnCommand);

}; // class CommandVisitor
} // namespace app
} // namespace mach

#endif // #ifndef MACHIAVELLI_MACH_APP_COMMANDVISITOR_H_INCLUDED
