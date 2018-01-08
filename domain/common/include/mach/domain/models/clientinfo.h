#ifndef MACHIAVELLI_MACH_DOMAIN_MODELS_CLIENTINFO_H_INCLUDED
#define MACHIAVELLI_MACH_DOMAIN_MODELS_CLIENTINFO_H_INCLUDED

#include <string>

namespace mach
{
namespace domain
{
namespace models
{
struct ClientInfo
{
    unsigned int id;
    std::string source;
}; // struct ClientInfo
} // namespace models
} // namespace domain
} // namespace mach

#endif // #ifndef MACHIAVELLI_MACH_DOMAIN_MODELS_CLIENTINFO_H_INCLUDED
