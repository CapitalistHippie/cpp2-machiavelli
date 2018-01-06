#pragma once
#include "mach\domain\models\player.h"
#include <deque>
#include <string>
#include <vector>

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
};

std::ostream& operator<<(std::ostream& os, const Game& game);
std::istream& operator>>(std::istream& is, Game& game);
