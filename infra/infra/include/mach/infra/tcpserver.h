#ifndef MACHIAVELLI_MACH_INFRA_TCPSERVER_H_INCLUDED
#define MACHIAVELLI_MACH_INFRA_TCPSERVER_H_INCLUDED

#include "mach/infra/noncopyable.h"
#include "mach/infra/sockets.h"
#include "mach/infra/tcpclient.h"

namespace mach
{
namespace infra
{
class TcpServer : public Noncopyable
{
  private:
    bool isListening;

    Socket listeningSocket;

  public:
    TcpServer();
    TcpServer(TcpServer&& other);
    TcpServer& operator=(TcpServer&& other);
    ~TcpServer();

    void StartListening(Port port);
    void StopListening();
    bool IsListening();

    TcpClient AcceptClient();
}; // class TcpServer
} // namespace infra
} // namespace mach

#endif // #ifndef MACHIAVELLI_MACH_INFRA_TCPSERVER_H_INCLUDED
