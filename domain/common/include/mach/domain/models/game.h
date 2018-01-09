#pragma once

#include <deque>
#include <string>
#include <vector>

#include "mach/dal/models/buildingcard.h"
#include "mach/dal/models/charactercard.h"
#include "mach/domain/gamestate.h"
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
    GameState state;
    bool playerReceivedGoldOrCards, playerUsedCharacterPower;
    int characterHasTurn;
    int buildingsStillAllowedToPlayThisTurn;
    int killedCharacter;
    Player king;

    std::vector<dal::models::CharacterCard> charactersToChooseFrom;
    std::vector<dal::models::CharacterCard> characters;
    std::deque<dal::models::BuildingCard> buildingCardStack;
    std::vector<Player> players;
    std::vector<std::string> playersWaiting;
    std::vector<std::pair<std::string, bool>> choosingTurns;

    models::Player& GetCurrentPlayer();
    std::string GetCurrentPlayerName() const;
    bool CharacterHasPlayer(int nr);

}; // class Game
} // namespace models
} // namespace domain
} // namespace mach

std::ostream& operator<<(std::ostream& os, const mach::domain::models::Game& game);
std::istream& operator>>(std::istream& is, mach::domain::models::Game& game);
