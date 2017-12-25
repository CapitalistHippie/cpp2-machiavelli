#include "mach/infra/tcpserver.h"

#include <system_error>

#include "mach/infra/functionalerror.h"
#include "mach/infra/functionalerrorcategory.h"

using namespace mach::infra;

std::unique_ptr<addrinfo, void(__stdcall*)(addrinfo*)> GetAddrInfoRaii(TcpServer::Port port, const addrinfo& hints)
{
    addrinfo* addrInfoResultBuffer = nullptr;
    std::unique_ptr<addrinfo, void(__stdcall*)(addrinfo*)> addrInfoResult(nullptr, &freeaddrinfo);

    auto result = getaddrinfo(nullptr, std::to_string(port).c_str(), &hints, &addrInfoResultBuffer);
    if (result != NOERROR)
    {
        throw std::system_error(result, std::system_category());
    }

    addrInfoResult.reset(addrInfoResultBuffer);

    return addrInfoResult;
}

TcpServer::TcpServer()
  : isListening(false)
  , listenSocket(INVALID_SOCKET)
{
#ifdef _WIN32
    InitializeWsa();
#endif // #ifdef _WIN32
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

    auto addrInfo = GetAddrInfoRaii(port, hints);

    // Create a socket for connecting to the server.
    listenSocket = socket(addrInfo->ai_family, addrInfo->ai_socktype, addrInfo->ai_protocol);
    if (listenSocket == INVALID_SOCKET)
    {
        throw std::system_error(WSAGetLastError(), std::system_category());
    }

    // Setup the TCP listening socket.
    auto result = bind(listenSocket, addrInfo->ai_addr, addrInfo->ai_addrlen);
    if (result != NOERROR)
    {
        closesocket(listenSocket);

        throw std::system_error(result, std::system_category());
    }

    // Start listening for connecting clients.
    result = listen(listenSocket, SOMAXCONN);
    if (result != NOERROR)
    {
        closesocket(listenSocket);

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

    auto result = closesocket(listenSocket);
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

TcpServerClient TcpServer::AcceptClient()
{
    if (!IsListening())
    {
        throw std::system_error(FunctionalError::TcpServerIsNotListening);
    }

    auto clientSocket = accept(listenSocket, nullptr, nullptr);
    if (clientSocket == INVALID_SOCKET)
    {
        throw std::system_error(WSAGetLastError(), std::system_category());
    }

    return TcpServerClient(clientSocket);
}
