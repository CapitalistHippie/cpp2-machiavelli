#ifndef MACHIAVELLI_MACH_VIEW_STATEHANDLERS_STATEHANDLER_H_INCLUDED
#define MACHIAVELLI_MACH_VIEW_STATEHANDLERS_STATEHANDLER_H_INCLUDED

#include "mach/view/serverclistate.h"

namespace mach
{
namespace view
{
namespace statehandlers
{
class StateHandler
{
  public:
    virtual ~StateHandler() = default;

    virtual void EnterState() = 0;
    virtual void ExitState() noexcept = 0;
    virtual void RenderConsole() const = 0;

    virtual ServerCliState GetState() const noexcept = 0;
};
}
}
}

#endif // MACHIAVELLI_MACH_VIEW_STATEHANDLERS_STATEHANDLER_H_INCLUDED
