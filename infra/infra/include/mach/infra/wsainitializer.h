#ifndef MACHIAVELLI_MACH_INFRA_WSAINITIALIZER_H_INCLUDED
#define MACHIAVELLI_MACH_INFRA_WSAINITIALIZER_H_INCLUDED

#ifdef _WIN32

namespace mach
{
namespace infra
{
void InitializeWsa() noexcept;
}
}

#endif // #ifdef _WIN32

#endif // #ifndef MACHIAVELLI_MACH_INFRA_WSAINITIALIZER_H_INCLUDED
