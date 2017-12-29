#ifndef MACHIAVELLI_MACH_APP_EVENTS_CLIENTCONNECTEDEVENT_H_INCLUDED
#define MACHIAVELLI_MACH_APP_EVENTS_CLIENTCONNECTEDEVENT_H_INCLUDED

#include <mach/infra/observable.h>

#include "mach/app/serverclient.h"

namespace mach
{
namespace app
{
namespace events
{
class ClientConnectedEvent : public infra::Observable
{
  public:
    const ServerClient& client;

    ClientConnectedEvent(const ServerClient& client)
      : client(client)
    {
    }
};
}
}
}

#endif // MACHIAVELLI_MACH_APP_EVENTS_CLIENTCONNECTEDEVENT_H_INCLUDED
