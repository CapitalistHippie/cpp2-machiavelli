#pragma once

#include <deque>
#include <string>
#include <vector>

#include "mach/domain/models/player.h"

namespace mach
{
namespace domain
{
namespace models
{
class Game
{
  public:
    bool isStarted;
    int characterHasTurn;
    Player king;

    std::vector<CharacterCard> characters;
    std::deque<BuildingCard> buildingCardStack;
    std::vector<Player> players;
    std::vector<std::string> playersWaiting;
}; // class Game
} // namespace models
} // namespace domain
} // namespace mach

std::ostream& operator<<(std::ostream& os, const mach::domain::models::Game& game);
std::istream& operator>>(std::istream& is, mach::domain::models::Game& game);
