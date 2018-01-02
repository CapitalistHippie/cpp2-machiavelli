#include "mach/infra/socketerrorcategory.h"

using namespace mach;
using namespace mach::infra;

const char* SocketErrorCategory::name() const noexcept
{
    return "Machiavelli Infrastructure Socket Error Category";
}

std::string SocketErrorCategory::message(int ev) const
{
    switch (static_cast<SocketError>(ev))
    {
        default:
            return "Unknown error.";
    }
}

const std::error_category& mach::infra::GetSocketErrorCategory()
{
    static SocketErrorCategory instance;
    return instance;
}

std::error_code std::make_error_code(SocketError error)
{
    return std::error_code(static_cast<int>(error), mach::infra::GetSocketErrorCategory());
}
