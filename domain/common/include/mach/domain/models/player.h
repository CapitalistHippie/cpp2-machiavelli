#pragma once

#include <string>
#include <vector>

#include "mach\dal\models\buildingcard.h"
#include "mach\dal\models\charactercard.h"

namespace mach
{
namespace domain
{
namespace models
{
class Player
{
  public:
    Player();
    Player(std::string name);

    std::string name;
    bool firstToHaveEightBuildings = false;
    int gold;
    std::vector<dal::models::BuildingCard> hand;
    std::vector<dal::models::BuildingCard> buildings;
    std::vector<dal::models::CharacterCard> characters;

    int GetAmountOfBuildingsByColor(dal::models::BuildingColor color) const;
    int GetPointsFromBuildings() const;
    int GetPoints() const;
}; // class Player
} // namespace models
} // namespace domain
} // namespace mach

std::ostream& operator<<(std::ostream& os, const mach::domain::models::Player& player);
std::istream& operator>>(std::istream& is, mach::domain::models::Player& player);
