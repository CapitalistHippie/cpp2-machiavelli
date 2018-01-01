#ifndef MACHIAVELLI_MACH_APP_MODELS_CLIENTINFO_H_INCLUDED
#define MACHIAVELLI_MACH_APP_MODELS_CLIENTINFO_H_INCLUDED

#include <string>

namespace mach
{
namespace app
{
namespace models
{
struct ClientInfo
{
    unsigned int id;
    std::string source;
}; // struct ClientInfo
} // namespace models
} // namespace app
} // namespace mach

#endif // #ifndef MACHIAVELLI_MACH_APP_MODELS_CLIENTINFO_H_INCLUDED
