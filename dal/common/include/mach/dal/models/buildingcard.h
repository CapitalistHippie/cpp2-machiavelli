#ifndef MACHIAVELLI_MACH_DAL_MODELS_BUILDINGCARD_H_INCLUDED
#define MACHIAVELLI_MACH_DAL_MODELS_BUILDINGCARD_H_INCLUDED

#include <string>

#include "mach/dal/models/buildingcolor.h"

namespace mach
{
namespace dal
{
namespace models
{
struct BuildingCard
{
    std::string name;
    int cost;
    BuildingColor color;
    std::string description;
}; // class BuildingCard
} // namespace models
} // namespace dal
} // namespace mach

#endif // #ifndef MACHIAVELLI_MACH_DAL_MODELS_BUILDINGCARD_H_INCLUDED
