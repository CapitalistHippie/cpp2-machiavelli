#include "mach/view/technicalerrorcategory.h"

using namespace mach;
using namespace mach::view;

const char* TechnicalErrorCategory::name() const noexcept
{
    return "Machiavelli View Technical Error Category";
}

std::string TechnicalErrorCategory::message(int ev) const
{
    switch (static_cast<TechnicalError>(ev))
    {
        case TechnicalError::None:
            return "No error.";
        case TechnicalError::NoStateHandler:
            return "No state handler.";
        default:
            return "Unknown error.";
    }
}

const std::error_category& mach::view::GetTechnicalErrorCategory()
{
    static TechnicalErrorCategory instance;
    return instance;
}

std::error_code std::make_error_code(TechnicalError error)
{
    return std::error_code(static_cast<int>(error), mach::view::GetTechnicalErrorCategory());
}
