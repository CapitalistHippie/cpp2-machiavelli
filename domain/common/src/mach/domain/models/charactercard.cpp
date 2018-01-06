#include "mach\domain\models\charactercard.h"

CharacterCard::CharacterCard()
{
}

CharacterCard::CharacterCard(int nr, std::string name)
  : nr{ nr }
  , name{ name }
{
}

CharacterCard::~CharacterCard()
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
    // if (owner == nullptr)
    //{
    //    return;
    //}
    //// TODO remove fugly switch case
    //// also... this probably shouldn't be here but in a controller?
    // switch (nr)
    //{
    //    case 1: // Moordenaar
    //            // TODO choose a character and kill
    //        break;
    //    case 2: // Dief
    //        // TODO choose a character and steal all their gold
    //        break;
    //    case 3: // Magiër
    //        // TODO choose:
    //        // 1. Swap hands with another player
    //        // 2. Swap any number of cards for others from the stack
    //        break;
    //    case 4: // Koning
    //        owner->gold += owner->GetAmountOfBuildingsByColor(BuildingColor::Yellow);
    //        break;
    //    case 5: // Prediker
    //        owner->gold += owner->GetAmountOfBuildingsByColor(BuildingColor::Blue);
    //        break;
    //    case 6: // Koopman
    //        owner->gold += 1 + owner->GetAmountOfBuildingsByColor(BuildingColor::Green);
    //        break;
    //    case 7: // Bouwmeester
    //        // TODO: trek 2 bouwkaarten
    //        owner->buildingsStillAllowedToPlay += 2;
    //        break;
    //    case 8: // Condotièrre
    //        // TODO verwijder gebouwen van andere spelers
    //        owner->gold += 1 + owner->GetAmountOfBuildingsByColor(BuildingColor::Red);
    //        break;
    //}
}
