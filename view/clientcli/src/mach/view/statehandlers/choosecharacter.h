#ifndef MACHIAVELLI_MACH_VIEW_STATEHANDLERS_CHOOSECHARACTER_H_INCLUDED
#define MACHIAVELLI_MACH_VIEW_STATEHANDLERS_CHOOSECHARACTER_H_INCLUDED

#include <mach/infra/clicommand.h>

#include "mach/view/clientclistate.h"
#include "mach/view/statehandlers/statehandlerbase.h"

namespace mach
{
namespace view
{
namespace statehandlers
{
class ChooseCharacter : public StateHandlerBase<ChooseCharacter, ClientCliState::ChooseCharacter>
{
  public:
    using StateHandlerBase::StateHandlerBase;

    void EnterState() override
    {
    }
}; // class WaitForGameToStart
} // namespace statehandlers
} // namespace view
} // namespace mach

#endif // #ifndef MACHIAVELLI_MACH_VIEW_STATEHANDLERS_CHOOSECHARACTER_H_INCLUDED
