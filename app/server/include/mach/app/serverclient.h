#ifndef MACHIAVELLI_MACH_APP_SERVERCLIENT_H_INCLUDED
#define MACHIAVELLI_MACH_APP_SERVERCLIENT_H_INCLUDED

#include <string>
#include <utility>

#include <mach/infra/concurrentautoincrement.h>
#include <mach/infra/tcpclient.h>

namespace mach
{
namespace app
{
class ServerClient
{
  private:
    infra::TcpClient tcpClient;

  public:
    typedef unsigned int Id;

    infra::ConcurrentAutoIncrement<Id> id;

    ServerClient(infra::TcpClient tcpClient);

    std::string GetSource() const;
};
}
}

#endif // MACHIAVELLI_MACH_APP_SERVERCLIENT_H_INCLUDED
