#include "mach/infra/tcpserverclient.h"

using namespace mach::infra;

TcpServerClient::TcpServerClient(SOCKET socket)
  : clientSocket(socket)
{
}
