#ifndef MACHIAVELLI_MACH_INFRA_SERIALIZABLE_H_INCLUDED
#define MACHIAVELLI_MACH_INFRA_SERIALIZABLE_H_INCLUDED

#include <istream>
#include <ostream>
#include <string>

namespace mach
{
namespace infra
{
class Serializable
{
  public:
    virtual ~Serializable() = default;

    virtual void Serialize(std::ostream& dataStream) const = 0;
    virtual void Deserialize(std::istream& dataStream) = 0;
    virtual std::string GetIdentifier() const = 0;
    virtual unsigned int GetDataLength() const = 0;
}; // class Serializable
} // namespace infra
} // namespace mach

#endif // #ifndef MACHIAVELLI_MACH_INFRA_SERIALIZABLE_H_INCLUDED
