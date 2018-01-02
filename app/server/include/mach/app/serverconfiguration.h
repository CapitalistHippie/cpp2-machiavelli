#ifndef MACHIAVELLI_MACH_APP_SERVERCONFIGURATION_H_INCLUDED
#define MACHIAVELLI_MACH_APP_SERVERCONFIGURATION_H_INCLUDED

#include <mach/app/constants.h>
#include <mach/infra/socket.h>

namespace mach
{
namespace app
{
struct ServerConfiguration
{
    static const unsigned int DefaultPlayerCount = 4;

    infra::Port port = Constants::DefaultTcpPort;
    unsigned int playerCount = DefaultPlayerCount;
}; // struct ServerConfiguration
} // namespace app
} // namespace mach

#endif // #ifndef MACHIAVELLI_MACH_APP_SERVERCONFIGURATION_H_INCLUDED
