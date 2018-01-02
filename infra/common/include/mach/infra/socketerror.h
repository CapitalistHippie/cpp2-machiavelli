#ifndef MACHIAVELLI_MACH_INFRA_SOCKETERROR_H_INCLUDED
#define MACHIAVELLI_MACH_INFRA_SOCKETERROR_H_INCLUDED

namespace mach
{
namespace infra
{
enum class SocketError
{
    None,
    Wouldblock,
    Again,
    Connreset,
    Other
}; // enum class SocketError
} // namespace infra
} // namespace mach

#endif // #ifndef MACHIAVELLI_MACH_INFRA_SOCKETERROR_H_INCLUDED
