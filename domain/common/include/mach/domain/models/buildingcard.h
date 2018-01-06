#pragma once
#include "mach\domain\models\buildingcolor.h"
#include <ostream>
#include <string>

using namespace mach::domain;

class BuildingCard
{
  public:
    BuildingCard();
    BuildingCard(int cost, std::string name, BuildingColor color, std::string description);
    ~BuildingCard();

    int cost;
    std::string name;
    std::string description;
    BuildingColor color;
};
std::ostream& operator<<(std::ostream& os, const BuildingCard& card);
std::istream& operator>>(std::istream& is, BuildingCard& card);
