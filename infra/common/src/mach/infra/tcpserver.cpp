#include "mach/infra/tcpserver.h"

using namespace mach::infra;

TcpServer::TcpServer(ThreadPool& threadPool)
  : threadPool(threadPool)
  , isListening(false)
  , listeningSocket(InvalidSocket)
{
}

mach::infra::TcpServer::TcpServer(TcpServer&& other)
  : threadPool(other.threadPool)
  , isListening(other.isListening)
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
        throw std::system_error(GetLastSocketErrorCode());
    }

    // Setup the TCP listening socket.
    auto result = bind(listeningSocket, addrInfo->ai_addr, addrInfo->ai_addrlen);
    if (result == SocketErrorResult)
    {
        CloseSocket(listeningSocket);

        throw std::system_error(ConvertSystemSocketErrorCode(result));
    }

    // Start listening for connecting clients.
    result = listen(listeningSocket, SOMAXCONN);
    if (result == SocketErrorResult)
    {
        CloseSocket(listeningSocket);

        throw std::system_error(ConvertSystemSocketErrorCode(result));
    }

    isListening = true;
}

void TcpServer::StopListening()
{
    if (!IsListening())
    {
        return;
    }

    CloseSocket(listeningSocket);

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

    SetSocketNonBlockingMode(listeningSocket, false);

    auto clientSocket = accept(listeningSocket, nullptr, nullptr);
    if (IsInvalidSocket(clientSocket))
    {
        throw std::system_error(GetLastSocketErrorCode());
    }

    return TcpClient(clientSocket);
}
