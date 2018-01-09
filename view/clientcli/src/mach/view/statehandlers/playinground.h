#ifndef MACHIAVELLI_MACH_VIEW_STATEHANDLERS_PLAYINGROUND_H_INCLUDED
#define MACHIAVELLI_MACH_VIEW_STATEHANDLERS_PLAYINGROUND_H_INCLUDED

#include <mach/infra/clicommand.h>

#include "mach/view/clientclistate.h"
#include "mach/view/statehandlers/statehandlerbase.h"

namespace mach
{
namespace view
{
namespace statehandlers
{
class PlayingRound : public StateHandlerBase<PlayingRound, ClientCliState::PlayingRound>
{
  public:
    using StateHandlerBase::StateHandlerBase;

    void EnterState() override;

  private:
    void GetGoldCommandHandler();
    void GetCardCommandHandler();
    void UseCharacterPowerCommandHandler();
    void BuildBuildingCommandHandler(const infra::CliCommand& command);
    void ChooseCardCommandHandler(const infra::CliCommand& command);
    void EndTurnCommandHandler();
    void PrintGameStatus(domain::models::Game game);
    bool myTurn;

    std::string ColorToString(dal::models::BuildingColor color);

}; // class PlayingRound
} // namespace statehandlers
} // namespace view
} // namespace mach

#endif // #ifndef MACHIAVELLI_MACH_VIEW_STATEHANDLERS_WAITFORGAMETOSTART_H_INCLUDED
