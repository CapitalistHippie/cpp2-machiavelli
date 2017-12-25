#ifndef MACHIAVELLI_MACH_APP_SERVERCLIENT_H_INCLUDED
#define MACHIAVELLI_MACH_APP_SERVERCLIENT_H_INCLUDED

#include <string>
#include <utility>

#include <mach/infra/tcpserverclient.h>

namespace mach
{
namespace app
{
class ServerClient
{
  private:
    infra::TcpServerClient tcpClient;

  public:
    ServerClient(infra::TcpServerClient tcpClient);

    std::string GetSource() const;
};
}
}

#endif // MACHIAVELLI_MACH_APP_SERVERCLIENT_H_INCLUDED
