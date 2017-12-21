#ifndef MACHIAVELLI_MACH_INFRA_FUNCTIONALERROR_H_INCLUDED
#define MACHIAVELLI_MACH_INFRA_FUNCTIONALERROR_H_INCLUDED

namespace mach
{
namespace infra
{
enum class FunctionalError
{
    None,
    CliCommandNotRegistered,
    CliCommandInvalidParameterInputType
};
}
}

#endif // MACHIAVELLI_MACH_INFRA_FUNCTIONALERROR_H_INCLUDED
