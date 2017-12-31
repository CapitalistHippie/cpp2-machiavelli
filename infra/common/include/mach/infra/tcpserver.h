#ifndef MACHIAVELLI_MACH_INFRA_TCPSERVER_H_INCLUDED
#define MACHIAVELLI_MACH_INFRA_TCPSERVER_H_INCLUDED

#include <chrono>
#include <functional>
#include <system_error>
#include <thread>

#include "mach/infra/functionalerror.h"
#include "mach/infra/functionalerrorcategory.h"
#include "mach/infra/noncopyable.h"
#include "mach/infra/sockets.h"
#include "mach/infra/tcpclient.h"
#include "mach/infra/threadpool.h"

namespace mach
{
namespace infra
{
class TcpServer : public Noncopyable
{
  private:
    ThreadPool& threadPool;

    bool isListening;

    Socket listeningSocket;

  public:
    TcpServer(ThreadPool& threadPool);
    TcpServer(TcpServer&& other);
    TcpServer& operator=(TcpServer&& other);
    ~TcpServer();

    void StartListening(Port port);
    void StopListening();
    bool IsListening();

    TcpClient AcceptClient();

    template<typename TCallback>
    void AcceptClientAsync(TCallback callback)
    {
        if (!IsListening())
        {
            throw std::system_error(FunctionalError::TcpServerIsNotListening);
        }

        threadPool.QueueTask([=]() {
            while (true)
            {
                if (!IsListening())
                {
                    return;
                }

                SetSocketNonBlockingMode(listeningSocket, true);

                auto clientSocket = accept(listeningSocket, nullptr, nullptr);

                if (IsInvalidSocket(clientSocket))
                {
                    auto errorCode = GetLastSocketErrorCode();
#ifdef _WIN32
                    if (errorCode == WSAEWOULDBLOCK)
#else // Assuming POSIX.
                    if (errorCode == EAGAIN || errorCode == EWOULDBLOCK)
#endif
                    {
                        std::this_thread::sleep_for(std::chrono::milliseconds(100));
                    }
                    else
                    {
                        throw std::system_error(errorCode, std::system_category());
                    }
                }
                else
                {
                    callback(TcpClient(clientSocket));
                    break;
                }
            }
        });
    }
}; // class TcpServer
} // namespace infra
} // namespace mach

#endif // #ifndef MACHIAVELLI_MACH_INFRA_TCPSERVER_H_INCLUDED
