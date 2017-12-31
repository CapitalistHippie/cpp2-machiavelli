#ifndef MACHIAVELLI_MACH_INFRA_FUNCTIONALERRORCATEGORY_H_INCLUDED
#define MACHIAVELLI_MACH_INFRA_FUNCTIONALERRORCATEGORY_H_INCLUDED

#include <system_error>

#include "mach/infra/functionalerror.h"

namespace mach
{
namespace infra
{
class FunctionalErrorCategory : public std::error_category
{
  public:
    virtual const char* name() const noexcept;
    virtual std::string message(int ev) const;
}; // class FunctionalErrorCategory

const std::error_category& GetFunctionalErrorCategory();
} // namespace infra
} // namespace mach

namespace std
{
template<>
struct is_error_code_enum<mach::infra::FunctionalError> : true_type
{
};

error_code make_error_code(mach::infra::FunctionalError);
} // namespace std

#endif // #ifndef MACHIAVELLI_MACH_INFRA_FUNCTIONALERRORCATEGORY_H_INCLUDED
