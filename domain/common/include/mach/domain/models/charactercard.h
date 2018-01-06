#pragma once
#include <string>

class CharacterCard
{
  public:
    CharacterCard();
    CharacterCard(int nr, std::string name);
    ~CharacterCard();
    void UseCharacterPower();

    int nr;
    std::string name;
};

std::ostream& operator<<(std::ostream& os, const CharacterCard& card);
std::istream& operator>>(std::istream& os, CharacterCard& card);
