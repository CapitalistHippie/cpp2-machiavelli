#ifndef MACHIAVELLI_MACH_INFRA_SOCKETERRORCATEGORY_H_INCLUDED
#define MACHIAVELLI_MACH_INFRA_SOCKETERRORCATEGORY_H_INCLUDED

#include <system_error>

#include "mach/infra/socketerror.h"

namespace mach
{
namespace infra
{
class SocketErrorCategory : public std::error_category
{
  public:
    virtual const char* name() const noexcept;
    virtual std::string message(int ev) const;
}; // class SocketErrorCategory

const std::error_category& GetSocketErrorCategory();
} // namespace infra
} // namespace mach

namespace std
{
template<>
struct is_error_code_enum<mach::infra::SocketError> : true_type
{
};

error_code make_error_code(mach::infra::SocketError);
} // namespace std

#endif // #ifndef MACHIAVELLI_MACH_INFRA_SOCKETERRORCATEGORY_H_INCLUDED
