#include "mach/infra/commandmediator.h"

using namespace mach::infra;
using namespace mach::infra::detail;

void CommandMediator::Clear()
{
    predicateHandlers.clear();
}
