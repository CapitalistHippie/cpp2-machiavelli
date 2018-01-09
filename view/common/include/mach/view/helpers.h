#ifndef MACHIAVELLI_MACH_VIEW_HELPERS_H_INCLUDED
#define MACHIAVELLI_MACH_VIEW_HELPERS_H_INCLUDED

#include <string>

#include <mach/dal/models/buildingcolor.h>

namespace mach
{
namespace view
{
std::string BuildingColorEnumToString(dal::models::BuildingColor buildingColor);
std::string BooleanToString(bool boolean);
} // namespace view
} // namespace mach

#endif // #ifndef MACHIAVELLI_MACH_VIEW_HELPERS_H_INCLUDED
