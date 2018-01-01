#ifndef MACHIAVELLI_MACH_INFRA_SERIALIZER_H_INCLUDED
#define MACHIAVELLI_MACH_INFRA_SERIALIZER_H_INCLUDED

#include <istream>
#include <memory>
#include <ostream>
#include <sstream>
#include <string>
#include <type_traits>

#include "mach/infra/abstractfactory.h"
#include "mach/infra/serializable.h"
#include "mach/infra/tcpclient.h"

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
    std::shared_ptr<TSerializable> DeserializeData(std::istream& dataStream, TIdentifier identifier)
    {
        auto& factory = infra::AbstractFactory<TSerializable, TIdentifier>::GetInstance();

        auto obj = factory.Construct(identifier);

        obj->Deserialize(dataStream);

        return obj;
    }

  public:
    std::string Serialize(const Serializable& serializable)
    {
        std::stringstream dataStream;

        dataStream << serializable.GetIdentifier() << ',' << serializable.GetDataLength() << ',';

        serializable.Serialize(dataStream);

        return dataStream.str();
    }

    template<typename TSerializable, typename TIdentifier>
    std::shared_ptr<TSerializable> Deserialize(std::istream& dataStream)
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
    std::shared_ptr<TSerializable> Deserialize(const TcpClient& tcpClient)
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
}; // class Serializer
} // namespace infra
} // namespace mach

#endif // #ifndef MACHIAVELLI_MACH_INFRA_SERIALIZER_H_INCLUDED