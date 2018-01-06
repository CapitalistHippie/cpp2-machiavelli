#pragma once

#include <ostream>
#include <string>

#include "mach/domain/models/buildingcolor.h"

namespace mach
{
namespace domain
{
namespace models
{
class BuildingCard
{
  public:
    BuildingCard();
    BuildingCard(int cost, std::string name, BuildingColor color, std::string description);

    int cost;
    std::string name;
    std::string description;
    BuildingColor color;
}; // class BuildingCard
} // namespace models
} // namespace domain
} // namespace mach

std::ostream& operator<<(std::ostream& os, const mach::domain::models::BuildingCard& card);
std::istream& operator>>(std::istream& is, mach::domain::models::BuildingCard& card);
