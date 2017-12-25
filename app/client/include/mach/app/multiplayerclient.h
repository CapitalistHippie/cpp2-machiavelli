#ifndef MACHIAVELLI_MACH_APP_MULTIPLAYERCLIENT_H_INCLUDED
#define MACHIAVELLI_MACH_APP_MULTIPLAYERCLIENT_H_INCLUDED

#include <string>

#include <mach/infra/tcpclient.h>

namespace mach
{
namespace app
{
class MultiplayerClient
{
  public:
    const static infra::Port DefaultPort = 4321; // TODO: Move this one to a global app library.

  private:
    infra::TcpClient tcpClient;

  public:
    void Connect(const std::string& hostname);
};
}
}

#endif // MACHIAVELLI_MACH_APP_MULTIPLAYERCLIENT_H_INCLUDED
