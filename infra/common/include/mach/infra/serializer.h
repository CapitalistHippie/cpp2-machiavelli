#ifndef MACHIAVELLI_MACH_INFRA_SERIALIZER_H_INCLUDED
#define MACHIAVELLI_MACH_INFRA_SERIALIZER_H_INCLUDED

#include <deque>
#include <istream>
#include <memory>
#include <ostream>
#include <sstream>
#include <string>
#include <system_error>
#include <type_traits>

#include "mach/infra/abstractfactory.h"
#include "mach/infra/serializable.h"
#include "mach/infra/tcpclient.h"

template<typename T>
std::ostream& operator<<(std::ostream& stream, const std::vector<T>& e)
{
    stream << e.size() << ',';
    for (auto item : e)
    {
        stream << item << ',';
    }
    return stream;
}

template<typename T>
std::istream& operator>>(std::istream& stream, std::vector<T>& e)
{
    e.clear();
    int size;
    stream >> size;
    stream.ignore();
    for (; size > 0; size--)
    {
        T t;
        stream >> t;
        stream.ignore();
        e.push_back(std::move(t));
    }
    return stream;
}

template<typename T>
std::ostream& operator<<(std::ostream& stream, const std::deque<T>& e)
{
    stream << e.size() << ',';
    for (auto item : e)
    {
        stream << item << ',';
    }
    return stream;
}

template<typename T>
std::istream& operator>>(std::istream& stream, std::deque<T>& e)
{
    e.clear();
    int size;
    stream >> size;
    stream.ignore();
    for (; size > 0; size--)
    {
        T t;
        stream >> t;
        stream.ignore();
        e.push_back(std::move(t));
    }
    return stream;
}

template<typename T>
std::ostream& operator<<(typename std::enable_if<std::is_enum<T>::value, std::ostream>::type& stream, const T& e)
{
    return stream << static_cast<typename std::underlying_type<T>::type>(e);
}

template<typename T>
std::istream& operator>>(typename std::enable_if<std::is_enum<T>::value, std::istream>::type& stream, T& e)
{
    typedef std::underlying_type<T>::type TEnum;

    return stream >> reinterpret_cast<TEnum&>(e);
}

namespace mach
{
namespace infra
{
class Serializer
{
  private:
    template<typename TSerializable, typename TIdentifier>
    std::shared_ptr<TSerializable> DeserializeData(std::istream& dataStream, TIdentifier identifier) const
    {
        auto& factory = infra::AbstractFactory<TSerializable, TIdentifier>::GetInstance();

        auto obj = factory.Construct(identifier);

        obj->Deserialize(dataStream);

        return obj;
    }

  public:
    std::string Serialize(const Serializable& serializable) const
    {
        std::stringstream dataStream;

        dataStream << serializable.GetIdentifier() << ',' << serializable.GetDataLength() << ',';

        serializable.Serialize(dataStream);

        return dataStream.str();
    }

    template<typename TSerializable, typename TIdentifier>
    std::shared_ptr<TSerializable> Deserialize(std::istream& dataStream) const
    {
        TIdentifier identifier;
        dataStream >> identifier;
        dataStream.ignore(1); // Ignore the ','.
        unsigned int dataLength;
        dataStream >> dataLength;
        dataStream.ignore(1); // Ignore the ','.

        return DeserializeData<TSerializable>(dataStream, identifier);
    }

    template<typename TSerializable, typename TIdentifier>
    std::shared_ptr<TSerializable> Deserialize(const TcpClient& tcpClient) const
    {
        std::stringstream dataStream;
        tcpClient.ReadUntilIncluding(',', dataStream);
        tcpClient.ReadUntilIncluding(',', dataStream);

        TIdentifier identifier;
        dataStream >> identifier;
        dataStream.ignore(1); // Ignore the ','.
        unsigned int dataLength;
        dataStream >> dataLength;
        dataStream.ignore(1); // Ignore the ','.

        tcpClient.Read(dataLength, dataStream);
        return DeserializeData<TSerializable>(dataStream, identifier);
    }

    template<typename TSerializable, typename TIdentifier, typename TCallback>
    void DeserializeAsync(const TcpClient& tcpClient, TCallback callback) const
    {
        auto dataStream = std::make_shared<std::stringstream>();

        tcpClient.ReadUntilIncludingAsync(',', *dataStream, [=, &tcpClient](std::error_code error) {
            if (error)
            {
                callback(std::move(error), std::shared_ptr<TSerializable>());
            }

            tcpClient.ReadUntilIncludingAsync(',', *dataStream, [=, &tcpClient](std::error_code error) {
                if (error)
                {
                    callback(std::move(error), std::shared_ptr<TSerializable>());
                }

                TIdentifier identifier;
                *dataStream >> identifier;
                dataStream->ignore(1); // Ignore the ','.
                unsigned int dataLength;
                *dataStream >> dataLength;
                dataStream->ignore(1); // Ignore the ','.

                tcpClient.ReadAsync(dataLength, *dataStream, [=, &tcpClient](std::error_code error) {
                    if (error)
                    {
                        callback(std::move(error), std::shared_ptr<TSerializable>());
                    }

                    callback(std::error_code(), DeserializeData<TSerializable>(*dataStream, identifier));
                });
            });
        });
    }
}; // class Serializer
} // namespace infra
} // namespace mach

#endif // #ifndef MACHIAVELLI_MACH_INFRA_SERIALIZER_H_INCLUDED
