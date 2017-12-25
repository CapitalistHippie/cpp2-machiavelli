#ifndef MACHIAVELLI_MACH_APP_SERVERCONFIGURATION_H_INCLUDED
#define MACHIAVELLI_MACH_APP_SERVERCONFIGURATION_H_INCLUDED

#include <mach/infra/sockets.h>

namespace mach
{
namespace app
{
struct ServerConfiguration
{
    static const infra::Port DefaultPort = 4321;
    static const unsigned int DefaultPlayerCount = 2;

    infra::Port port = DefaultPort;
    unsigned int playerCount = DefaultPlayerCount;
};
}
}

#endif // MACHIAVELLI_MACH_APP_SERVERCONFIGURATION_H_INCLUDED
