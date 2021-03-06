#ifndef MACHIAVELLI_MACH_INFRA_TCPSERVER_H_INCLUDED
#define MACHIAVELLI_MACH_INFRA_TCPSERVER_H_INCLUDED

#include <chrono>
#include <functional>
#include <system_error>
#include <thread>

#include "mach/infra/functionalerror.h"
#include "mach/infra/functionalerrorcategory.h"
#include "mach/infra/noncopyable.h"
#include "mach/infra/socket.h"
#include "mach/infra/socketerrorcategory.h"
#include "mach/infra/tcpclient.h"
#include "mach/infra/threadpool.h"

namespace mach
{
namespace infra
{
class TcpServer : public Noncopyable
{
  private:
    ThreadPool* threadPool;

    bool isListening;

    Socket listeningSocket;

    template<typename TCallback>
    void AcceptClientAsyncImpl(TCallback callback)
    {
        threadPool->QueueTask([=]() {
            if (!IsListening())
            {
                return;
            }

            SetSocketNonBlockingMode(listeningSocket, true);

            auto clientSocket = accept(listeningSocket, nullptr, nullptr);

            if (IsInvalidSocket(clientSocket))
            {
                auto errorCode = GetLastSocketErrorCode();

                if (errorCode == SocketError::Wouldblock || errorCode == SocketError::Again)
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                    AcceptClientAsyncImpl(callback);

                    return;
                }
                else
                {
                    throw std::system_error(errorCode);
                }
            }
            else
            {
                callback(TcpClient(*threadPool, clientSocket));
            }
        });
    }

  public:
    TcpServer(ThreadPool& threadPool);
    TcpServer(TcpServer&& other);
    TcpServer& operator=(TcpServer&& other);

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

        AcceptClientAsyncImpl(callback);
    }
}; // class TcpServer
} // namespace infra
} // namespace mach

#endif // #ifndef MACHIAVELLI_MACH_INFRA_TCPSERVER_H_INCLUDED
