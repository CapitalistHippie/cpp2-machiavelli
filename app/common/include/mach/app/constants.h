#ifndef MACHIAVELLI_MACH_APP_CONSTANTS_H_INCLUDED
#define MACHIAVELLI_MACH_APP_CONSTANTS_H_INCLUDED

#include <mach/infra/sockets.h>

namespace mach
{
namespace app
{
struct Constants
{
    const static infra::Port DefaultTcpPort = 4321;
}; // struct Constants
} // namespace app
} // namespace mach

#endif // #ifndef MACHIAVELLI_MACH_APP_CONSTANTS_H_INCLUDED
