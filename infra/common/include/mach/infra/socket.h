#ifndef MACHIAVELLI_MACH_INFRA_SOCKET_H_INCLUDED
#define MACHIAVELLI_MACH_INFRA_SOCKET_H_INCLUDED

#include <cstdint>
#include <memory>
#include <string>

#ifdef _WIN32
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif
#include <winsock2.h>
#include <ws2tcpip.h>

typedef SOCKET Socket;

#define InvalidSocket INVALID_SOCKET
#define SocketErrorResult SOCKET_ERROR

#else // Assuming POSIX.
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

typedef int Socket;

#define InvalidSocket -1
#define SocketError -1

#endif

#include "mach/infra/socketerror.h"

namespace mach
{
namespace infra
{
typedef uint16_t Port;

bool IsInvalidSocket(Socket socket);

std::unique_ptr<addrinfo, void(__stdcall*)(addrinfo*)> GetAddrInfo(const char* hostname,
                                                                   const char* service,
                                                                   const addrinfo& hints);
std::unique_ptr<addrinfo, void(__stdcall*)(addrinfo*)> GetAddrInfo(const char* hostname,
                                                                   Port port,
                                                                   const addrinfo& hints);
void SetSocketNonBlockingMode(Socket socket, bool enabled);

void CloseSocket(Socket socket);

SocketError GetLastSocketErrorCode();

SocketError ConvertSystemSocketErrorCode(int errorCode);
} // namespace infra
} // namespace mach

#endif // #ifndef MACHIAVELLI_MACH_INFRA_SOCKET_H_INCLUDED
