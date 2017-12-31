#ifndef MACHIAVELLI_MACH_APP_MODELS_CLIENTINFO_H_INCLUDED
#define MACHIAVELLI_MACH_APP_MODELS_CLIENTINFO_H_INCLUDED

#include <string>

namespace mach
{
namespace app
{
namespace models
{
class ClientInfo
{
  public:
    unsigned int id;
    std::string source;
};
}
}
}

#endif // MACHIAVELLI_MACH_APP_MODELS_CLIENTINFO_H_INCLUDED
