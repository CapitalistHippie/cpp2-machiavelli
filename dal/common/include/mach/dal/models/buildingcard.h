#ifndef MACHIAVELLI_MACH_DAL_MODELS_BUILDINGCARD_H_INCLUDED
#define MACHIAVELLI_MACH_DAL_MODELS_BUILDINGCARD_H_INCLUDED

#include <istream>
#include <ostream>
#include <string>

#include "mach/dal/models/buildingcolor.h"
#include "mach/infra/serializer.h"

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

inline std::ostream& operator<<(std::ostream& os, const mach::dal::models::BuildingCard& card)
{
    return os << card.cost << '|' << card.name << '|' << card.description << '|' << card.color << '|';
}
inline std::istream& operator>>(std::istream& is, mach::dal::models::BuildingCard& card)
{
    is >> card.cost;
    is.ignore();
    std::getline(is, card.name, '|');
    std::getline(is, card.description, '|');
    is >> card.color;
    is.ignore();
    return is;
}

#endif // #ifndef MACHIAVELLI_MACH_DAL_MODELS_BUILDINGCARD_H_INCLUDED
