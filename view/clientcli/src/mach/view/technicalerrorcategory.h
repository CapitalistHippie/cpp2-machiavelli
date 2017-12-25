#ifndef MACHIAVELLI_MACH_VIEW_TECHNICALERRORCATEGORY_H_INCLUDED
#define MACHIAVELLI_MACH_VIEW_TECHNICALERRORCATEGORY_H_INCLUDED

#include <system_error>

#include "mach/view/technicalerror.h"

namespace mach
{
namespace view
{
class TechnicalErrorCategory : public std::error_category
{
  public:
    virtual const char* name() const noexcept;
    virtual std::string message(int ev) const;
}; // class TechnicalErrorCategory

const std::error_category& GetTechnicalErrorCategory();
} // namespace view
} // namespace mach

namespace std
{
template<>
struct is_error_code_enum<mach::view::TechnicalError> : true_type
{
};

error_code make_error_code(mach::view::TechnicalError);
} // namespace std

#endif // MACHIAVELLI_MACH_VIEW_TECHNICALERRORCATEGORY_H_INCLUDED
