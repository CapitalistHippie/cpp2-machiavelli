#include "mach/infra/tcpserver.h"

#include <system_error>

#include "mach/infra/functionalerror.h"
#include "mach/infra/functionalerrorcategory.h"

using namespace mach::infra;

TcpServer::TcpServer()
  : isListening(false)
  , listeningSocket(InvalidSocket)
{
}

mach::infra::TcpServer::TcpServer(TcpServer&& other)
  : isListening(other.isListening)
  , listeningSocket(other.listeningSocket)
{
    other.isListening = false;
    other.listeningSocket = InvalidSocket;
}

TcpServer& mach::infra::TcpServer::operator=(TcpServer&& other)
{
    isListening = other.isListening;
    listeningSocket = other.listeningSocket;

    other.isListening = false;
    other.listeningSocket = InvalidSocket;

    return *this;
}

mach::infra::TcpServer::~TcpServer()
{
    StopListening();
}

void TcpServer::StartListening(Port port)
{
    if (IsListening())
    {
        return;
    }

    // Resolve the server address and port.
    addrinfo hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    auto addrInfo = GetAddrInfo(nullptr, port, hints);

    // Create a socket for connecting to the server.
    listeningSocket = socket(addrInfo->ai_family, addrInfo->ai_socktype, addrInfo->ai_protocol);
    if (IsInvalidSocket(listeningSocket))
    {
        throw std::system_error(WSAGetLastError(), std::system_category());
    }

    // Setup the TCP listening socket.
    auto result = bind(listeningSocket, addrInfo->ai_addr, addrInfo->ai_addrlen);
    if (result != NOERROR)
    {
        CloseSocket(listeningSocket);

        throw std::system_error(result, std::system_category());
    }

    // Start listening for connecting clients.
    result = listen(listeningSocket, SOMAXCONN);
    if (result != NOERROR)
    {
        CloseSocket(listeningSocket);

        throw std::system_error(result, std::system_category());
    }

    isListening = true;
}

void TcpServer::StopListening()
{
    if (!IsListening())
    {
        return;
    }

    auto result = CloseSocket(listeningSocket);
    if (result != NOERROR)
    {
        throw std::system_error(result, std::system_category());
    }

    isListening = false;
}

bool TcpServer::IsListening()
{
    return isListening;
}

TcpClient TcpServer::AcceptClient()
{
    if (!IsListening())
    {
        throw std::system_error(FunctionalError::TcpServerIsNotListening);
    }

    auto clientSocket = accept(listeningSocket, nullptr, nullptr);
    if (IsInvalidSocket(clientSocket))
    {
        throw std::system_error(WSAGetLastError(), std::system_category());
    }

    return TcpClient(clientSocket);
}
