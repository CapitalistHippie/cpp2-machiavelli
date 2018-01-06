#pragma once

#include <string>

namespace mach
{
namespace domain
{
namespace models
{
class CharacterCard
{
  public:
    CharacterCard();
    CharacterCard(int nr, std::string name);

    void UseCharacterPower();

    int nr;
    std::string name;
}; // class CharacterCard
} // namespace models
} // namespace domain
} // namespace mach

std::ostream& operator<<(std::ostream& os, const mach::domain::models::CharacterCard& card);
std::istream& operator>>(std::istream& os, mach::domain::models::CharacterCard& card);
