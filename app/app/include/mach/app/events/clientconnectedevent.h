#ifndef MACHIAVELLI_MACH_APP_EVENTS_CLIENTCONNECTEDEVENT_H_INCLUDED
#define MACHIAVELLI_MACH_APP_EVENTS_CLIENTCONNECTEDEVENT_H_INCLUDED

#include <mach/infra/observable.h>

#include "mach/app/models/clientinfo.h"

namespace mach
{
namespace app
{
namespace events
{
class ClientConnectedEvent : public infra::Observable
{
  public:
    models::ClientInfo clientInfo;
};
}
}
}

#endif // MACHIAVELLI_MACH_APP_EVENTS_CLIENTCONNECTEDEVENT_H_INCLUDED
