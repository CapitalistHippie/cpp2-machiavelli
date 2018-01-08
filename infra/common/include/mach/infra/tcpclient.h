#ifndef MACHIAVELLI_MACH_INFRA_TCPCLIENT_H_INCLUDED
#define MACHIAVELLI_MACH_INFRA_TCPCLIENT_H_INCLUDED

#include <ostream>
#include <sstream>
#include <string>

#include "mach/infra/functionalerrorcategory.h"
#include "mach/infra/noncopyable.h"
#include "mach/infra/socket.h"
#include "mach/infra/socketerrorcategory.h"
#include "mach/infra/threadpool.h"

namespace mach
{
namespace infra
{
class TcpClient : public Noncopyable
{
  private:
    ThreadPool* threadPool;

    bool isConnected;

    Socket clientSocket;

    template<typename TCallback>
    void ReadAsyncImpl(unsigned int dataLength,
                       unsigned int dataReceived,
                       std::ostream& outputBuffer,
                       TCallback callback) const
    {
        if (!isConnected)
        {
            return;
        }

        threadPool->QueueTask([=, &outputBuffer]() {
            if (!isConnected)
            {
                return;
            }

            char dataBuffer[65];
            unsigned int totalDataReceived = dataReceived;

            int dataLeftToReceive = dataLength - totalDataReceived;
            int dataToReceive = 0;

            if (dataLeftToReceive > 64)
            {
                dataToReceive = 64;
            }
            else
            {
                dataToReceive = dataLeftToReceive;
            }

            SetSocketNonBlockingMode(clientSocket, true);

            auto dataReceivedAgain = recv(clientSocket, dataBuffer, dataToReceive, 0);

            if (dataReceivedAgain == -1)
            {
                auto errorCode = GetLastSocketErrorCode();

                if (errorCode == SocketError::Wouldblock || errorCode == SocketError::Again)
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                    ReadAsyncImpl(dataLength, totalDataReceived, outputBuffer, callback);

                    return;
                }
                else
                {
                    throw std::system_error(errorCode);
                }
            }

            totalDataReceived += dataReceivedAgain;

            dataBuffer[dataReceivedAgain] = '\0';
            outputBuffer << dataBuffer;

            if (totalDataReceived == dataLength)
            {
                callback();
            }
            else
            {
                ReadAsyncImpl(dataLength, totalDataReceived, outputBuffer, callback);
            }
        });
    }

    template<typename TCallback>
    void ReadUntilIncludingAsyncImpl(char delimiter, std::ostream& outputBuffer, TCallback callback) const
    {
        if (!isConnected)
        {
            return;
        }

        threadPool->QueueTask([=, &outputBuffer]() {
            if (!isConnected)
            {
                return;
            }

            auto dataBuffer = std::make_shared<std::stringstream>();

            ReadAsyncImpl(1, 0, *dataBuffer, [=, &outputBuffer] {
                auto data = dataBuffer->str();

                outputBuffer << data[0];

                if (data[0] == delimiter)
                {
                    callback();
                }
                else
                {
                    ReadUntilIncludingAsyncImpl(delimiter, outputBuffer, callback);
                }
            });
        });
    }

  public:
    TcpClient(ThreadPool& threadPool);
    TcpClient(ThreadPool& threadPool, Socket socket);
    TcpClient(TcpClient&& other);
    TcpClient& operator=(TcpClient&& other);

    void Connect(const std::string& hostname, Port port);
    void Disconnect();
    bool IsConnected() const;

    void Write(const std::string& data) const;
    void Write(const char* data, unsigned int dataLength) const;

    void Read(unsigned int dataLength, std::ostream& outputBuffer) const;
    void ReadUntilIncluding(char delimiter, std::ostream& outputBuffer) const;

    template<typename TCallback>
    void ReadAsync(unsigned int dataLength, std::ostream& outputBuffer, TCallback callback) const
    {
        if (!IsConnected())
        {
            throw std::system_error(FunctionalError::TcpClientNotConnected);
        }

        ReadAsyncImpl(dataLength, 0, outputBuffer, callback);
    }

    template<typename TCallback>
    void ReadUntilIncludingAsync(char delimiter, std::ostream& outputBuffer, TCallback callback) const
    {
        if (!IsConnected())
        {
            throw std::system_error(FunctionalError::TcpClientNotConnected);
        }

        ReadUntilIncludingAsyncImpl(delimiter, outputBuffer, callback);
    }

    std::string GetPeerAddress() const;

}; // class TcpClient
} // namespace infra
} // namespace mach

#endif // #ifndef MACHIAVELLI_MACH_INFRA_TCPCLIENT_H_INCLUDED
