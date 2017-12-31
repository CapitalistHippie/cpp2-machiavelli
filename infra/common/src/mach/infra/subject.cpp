#include "mach/infra/subject.h"

using namespace mach::infra;

void Subject::UnregisterPredicateObserver(ObserverHandle handle)
{
    predicateObservers.erase(handle);
}

void Subject::UnregisterObserver(ObserverHandle handle)
{
    observers.erase(handle);
}

void Subject::Clear()
{
    predicateObservers.clear();
    observers.clear();
}
