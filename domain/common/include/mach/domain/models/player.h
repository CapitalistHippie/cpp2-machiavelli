#pragma once
#include "mach\domain\models\buildingcard.h"
#include "mach\domain\models\charactercard.h"
#include <string>
#include <vector>

class Player
{
  public:
    Player();
    Player(std::string name);

    std::string name;
    int buildingsStillAllowedToPlay;
    bool firstToHaveEightBuildings = false;
    int gold;
    std::vector<BuildingCard> hand;
    std::vector<BuildingCard> buildings;
    std::vector<CharacterCard> characters;

    void PlayBuilding(int nr);

    int GetAmountOfBuildingsByColor(BuildingColor color);
    int GetPointsFromBuildings();
    int GetPoints();
};
std::ostream& operator<<(std::ostream& os, const Player& player);
std::istream& operator>>(std::istream& is, Player& player);
