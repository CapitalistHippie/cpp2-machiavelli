#ifndef MACHIAVELLI_MACH_APP_CLIENT_H_INCLUDED
#define MACHIAVELLI_MACH_APP_CLIENT_H_INCLUDED

#include <mach/infra/subject.h>

namespace mach
{
namespace app
{
class Client
{
  public:
    infra::Subject eventSubject;

    virtual ~Client() noexcept = default;
    virtual void StartAsync() = 0;
}; // class Client
} // namespace app
} // namespace mach

#endif // #ifndef MACHIAVELLI_MACH_APP_CLIENT_H_INCLUDED
