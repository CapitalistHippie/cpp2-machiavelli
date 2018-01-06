#include "mach\domain\models\buildingcard.h"

BuildingCard::BuildingCard()
{
}

BuildingCard::BuildingCard(int cost, std::string name, BuildingColor color, std::string description)
  : cost{ cost }
  , name{ name }
  , color{ color }
  , description{ description }
{
}

BuildingCard::~BuildingCard()
{
}

std::ostream& operator<<(std::ostream& os, const BuildingCard& card)
{
    return os << card.cost << '|' << card.name << '|' << card.description << '|' << card.color << '|';
}

std::istream& operator>>(std::istream& is, BuildingCard& card)
{
    is >> card.cost;
    is.ignore();
    std::getline(is, card.name, '|');
    std::getline(is, card.description, '|');
    is >> card.color;
    is.ignore();
    return is;
}
