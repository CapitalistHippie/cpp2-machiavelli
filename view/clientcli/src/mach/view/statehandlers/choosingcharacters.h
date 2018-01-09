#ifndef MACHIAVELLI_MACH_VIEW_STATEHANDLERS_CHOOSINGCHARACTERS_H_INCLUDED
#define MACHIAVELLI_MACH_VIEW_STATEHANDLERS_CHOOSINGCHARACTERS_H_INCLUDED

#include <mach/infra/clicommand.h>

#include "mach/view/clientclistate.h"
#include "mach/view/statehandlers/statehandlerbase.h"

namespace mach
{
namespace view
{
namespace statehandlers
{
class ChoosingCharacters : public StateHandlerBase<ChoosingCharacters, ClientCliState::ChoosingCharacters>
{
  public:
    using StateHandlerBase::StateHandlerBase;

    void EnterState() override;

  private:
    void PrintOptions(domain::models::Game game);
    void ChooseCharacterCommandHandler(const infra::CliCommand& command);

}; // class ChoosingCharacters
} // namespace statehandlers
} // namespace view
} // namespace mach

#endif // #ifndef MACHIAVELLI_MACH_VIEW_STATEHANDLERS_WAITFORGAMETOSTART_H_INCLUDED
