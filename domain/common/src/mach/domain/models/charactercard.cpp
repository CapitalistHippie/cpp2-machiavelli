#include "mach/domain/models/charactercard.h"

using namespace mach::domain::models;

CharacterCard::CharacterCard()
{
}

CharacterCard::CharacterCard(int nr, std::string name)
  : nr{ nr }
  , name{ name }
{
}

std::ostream& operator<<(std::ostream& os, const CharacterCard& card)
{
    return os << card.nr << '|' << card.name << '|';
}

std::istream& operator>>(std::istream& is, CharacterCard& card)
{
    is >> card.nr;
    is.ignore();
    std::getline(is, card.name, '|');
    return is;
}

void CharacterCard::UseCharacterPower()
{
}
