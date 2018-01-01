#ifndef MACHIAVELLI_MACH_INFRA_TCPCLIENT_H_INCLUDED
#define MACHIAVELLI_MACH_INFRA_TCPCLIENT_H_INCLUDED

#include <ostream>
#include <sstream>
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

    void Write(const std::string& data) const;
    void Write(const char* data, unsigned int dataLength) const;

    void Read(unsigned int dataLength, std::ostream& outputBuffer) const;
    void ReadUntilIncluding(char delimiter, std::ostream& outputBuffer) const;

    std::string GetPeerAddress() const;

}; // class TcpClient
} // namespace infra
} // namespace mach

#endif // #ifndef MACHIAVELLI_MACH_INFRA_TCPCLIENT_H_INCLUDED
