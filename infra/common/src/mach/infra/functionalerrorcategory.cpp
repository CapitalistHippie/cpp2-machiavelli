#include "mach/infra/functionalerrorcategory.h"

using namespace mach;
using namespace mach::infra;

const char* FunctionalErrorCategory::name() const noexcept
{
    return "Machiavelli Infrastructure Functional Error Category";
}

std::string FunctionalErrorCategory::message(int ev) const
{
    switch (static_cast<FunctionalError>(ev))
    {
        case FunctionalError::None:
            return "No error.";
        case FunctionalError::CliCommandNotRegistered:
            return "Command not registered.";
        case FunctionalError::CliCommandInvalidParameterInputType:
            return "Invalid parameter input type.";
        case FunctionalError::TcpServerIsNotListening:
            return "TCP Server is not in a listening state.";
        case FunctionalError::TcpServerClientInitializedWithInvalidSocket:
            return "TCP Server Client initialized with invalid socket.";
        case FunctionalError::TcpClientAlreadyConnected:
            return "TCP Client is already connected to a server.";
        case FunctionalError::TcpClientNotConnected:
            return "TCP Client is not connected to a server.";
        default:
            return "Unknown error.";
    }
}

const std::error_category& mach::infra::GetFunctionalErrorCategory()
{
    static FunctionalErrorCategory instance;
    return instance;
}

std::error_code std::make_error_code(FunctionalError error)
{
    return std::error_code(static_cast<int>(error), mach::infra::GetFunctionalErrorCategory());
}
