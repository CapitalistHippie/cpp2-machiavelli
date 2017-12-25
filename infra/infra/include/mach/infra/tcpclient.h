#ifndef MACHIAVELLI_MACH_INFRA_TCPCLIENT_H_INCLUDED
#define MACHIAVELLI_MACH_INFRA_TCPCLIENT_H_INCLUDED

#include <string>

#include "mach/infra/noncopyable.h"
#include "mach/infra/sockets.h"

namespace mach
{
namespace infra
{
class TcpClient : public Noncopyable
{
  private:
    bool isConnected;

    Socket clientSocket;

  public:
    TcpClient();
    TcpClient(Socket socket);
    TcpClient(TcpClient&& other);
    TcpClient& operator=(TcpClient&& other);
    ~TcpClient();

    void Connect(const std::string& hostname, Port port);
    void Disconnect();
    bool IsConnected() const;

    std::string GetPeerAddress() const;

}; // class TcpClient
} // namespace infra
} // namespace mach

#endif // #ifndef MACHIAVELLI_MACH_INFRA_TCPCLIENT_H_INCLUDED
