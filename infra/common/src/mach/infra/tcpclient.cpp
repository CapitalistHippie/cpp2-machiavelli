#include "mach/infra/tcpclient.h"

#include <system_error>

#include "mach/infra/functionalerror.h"
#include "mach/infra/functionalerrorcategory.h"

using namespace mach::infra;

mach::infra::TcpClient::TcpClient()
  : isConnected(false)
  , clientSocket(InvalidSocket)
{
}

mach::infra::TcpClient::TcpClient(Socket socket)
  : isConnected(false)
  , clientSocket(socket)
{
    if (IsInvalidSocket(socket))
    {
        throw std::system_error(FunctionalError::TcpServerClientInitializedWithInvalidSocket);
    }
}

mach::infra::TcpClient::TcpClient(TcpClient&& other)
  : isConnected(other.isConnected)
  , clientSocket(other.clientSocket)
{
    other.isConnected = false;
    other.clientSocket = InvalidSocket;
}

TcpClient& mach::infra::TcpClient::operator=(TcpClient&& other)
{
    isConnected = other.isConnected;
    clientSocket = other.clientSocket;

    other.isConnected = false;
    other.clientSocket = InvalidSocket;

    return *this;
}

mach::infra::TcpClient::~TcpClient()
{
    Disconnect();
}

void TcpClient::Connect(const std::string& hostname, Port port)
{
    if (IsConnected())
    {
        throw std::system_error(FunctionalError::TcpClientAlreadyConnected);
    }

    // Resolve the server address and port.
    addrinfo hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    auto addrInfo = GetAddrInfo(hostname.c_str(), port, hints);

    // Attempt to connect to an address until one succeeds.
    for (auto addr = addrInfo.get(); addr != nullptr; addr = addr->ai_next)
    {
        // Create a socket for connecting to the server.
        clientSocket = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
        if (IsInvalidSocket(clientSocket))
        {
            throw std::system_error(GetLastSocketErrorCode(), std::system_category());
        }

        // Connect to the server.
        auto result = connect(clientSocket, addr->ai_addr, addr->ai_addrlen);
        if (result != NOERROR)
        {
            CloseSocket(clientSocket);
            clientSocket = InvalidSocket;

            continue;
        }
        break;
    }

    if (IsInvalidSocket(clientSocket))
    {
        throw std::system_error(GetLastSocketErrorCode(), std::system_category(), "Unable to connect to a server.");
    }

    isConnected = true;
}

void TcpClient::Disconnect()
{
    if (!IsConnected())
    {
        return;
    }

    CloseSocket(clientSocket);

    isConnected = false;
}

bool TcpClient::IsConnected() const
{
    return isConnected;
}

std::string TcpClient::GetPeerAddress() const
{
    sockaddr clientInfo = { 0 };
    int clientInfoSize = sizeof(clientInfo);

    auto result = getpeername(clientSocket, &clientInfo, &clientInfoSize);
    if (result != NOERROR)
    {
        throw std::system_error(result, std::system_category());
    }

    return inet_ntoa(reinterpret_cast<sockaddr_in&>(clientInfo).sin_addr);
}
