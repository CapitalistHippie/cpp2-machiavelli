#pragma once

#include <string>
#include <vector>

#include "mach\domain\models\buildingcard.h"
#include "mach\domain\models\charactercard.h"

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
    std::vector<BuildingCard> hand;
    std::vector<BuildingCard> buildings;
    std::vector<CharacterCard> characters;

    void PlayBuilding(int nr);

    int GetAmountOfBuildingsByColor(BuildingColor color);
    int GetPointsFromBuildings();
    int GetPoints();
}; // class Player
} // namespace models
} // namespace domain
} // namespace mach

std::ostream& operator<<(std::ostream& os, const mach::domain::models::Player& player);
std::istream& operator>>(std::istream& is, mach::domain::models::Player& player);
