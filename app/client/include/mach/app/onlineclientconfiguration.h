#ifndef MACHIAVELLI_MACH_APP_ONLINECLIENTCONFIGURATION_H_INCLUDED
#define MACHIAVELLI_MACH_APP_ONLINECLIENTCONFIGURATION_H_INCLUDED

#include <string>

#include <mach/app/constants.h>
#include <mach/infra/sockets.h>

namespace mach
{
namespace app
{
struct OnlineClientConfiguration
{
    std::string hostname = "localhost";
    infra::Port port = Constants::DefaultTcpPort;
}; // struct OnlineClientConfiguration
} // namespace app
} // namespace mach

#endif // #ifndef MACHIAVELLI_MACH_APP_ONLINECLIENTCONFIGURATION_H_INCLUDED
