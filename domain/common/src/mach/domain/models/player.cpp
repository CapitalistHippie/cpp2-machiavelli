#include "mach/domain/models/player.h"

#include <algorithm>
#include <iterator>
#include <set>

using namespace mach::domain::models;

Player::Player()
{
}

Player::Player(std::string name)
  : name{ name }
{
}

// void Player::AddCharacterCard(CharacterCard card)
//{
//    if (characters.size >= 2)
//    {
//        throw std::exception("You can't have more than 2 character cards");
//    }
//
//    characters.push_back(std::move(card));
//}
//
// void Player::AddBuildingCard(BuildingCard card)
//{
//    hand.push_back(std::move(card));
//}
//
// void Player::PlayBuilding(int nr)
//{
//    if (hand.at(nr)->cost > gold)
//    {
//        throw std::exception("You don't have enough gold to buy that!");
//    }
//    buildings.push_back(hand.at(nr));
//    hand.erase(hand.begin() + nr);
//}

int Player::GetAmountOfBuildingsByColor(BuildingColor color)
{
    return std::count_if(
      buildings.begin(), buildings.end(), [&color](BuildingCard card) { return card.color == color; });
}

int Player::GetPointsFromBuildings()
{
    int points = 0;
    for (auto b : buildings)
    {
        points += b.cost;
    }
    return points;
}

int Player::GetPoints()
{
    int points = 0;
    std::set<BuildingColor> colorSet;
    for (auto b : buildings)
    {
        points += b.cost;
        colorSet.insert(b.color);
    }
    // If a player has buildings of every color: +3 points
    if (colorSet.size() >= 5)
    {
        points += 3;
    }
    if (firstToHaveEightBuildings)
    {
        points += 4;
    }
    if (buildings.size() >= 8)
    {
        points += 2;
    }
    return points;
}

std::ostream& operator<<(std::ostream& os, const Player& player)
{
    return os << player.name << '|' << player.buildingsStillAllowedToPlay << '|' << player.gold << '|' << player.hand
              << '|' << player.buildings << '|' << player.characters << '|';
}

std::istream& operator>>(std::istream& is, Player& player)
{
    std::getline(is, player.name, '|');
    is >> player.buildingsStillAllowedToPlay;
    is.ignore();
    is >> player.gold;
    is.ignore();
    is >> player.hand;
    is.ignore();
    is >> player.buildings;
    is.ignore();
    is >> player.characters;
    is.ignore();
    return is;
}
