#pragma once

#include <memory>
#include <stack>
#include <string>
#include <vector>

#include <mach/infra/subject.h>

#include "mach/domain/models/game.h"
#include "mach/domain/models/player.h"

namespace mach
{
namespace domain
{
class GameController
{
  public:
    GameController();
    GameController(models::Game game);

    models::Game game;

    void AddPlayer(std::string player);
    void RemovePlayer(std::string playerName);

    void StartGame();
    void EndGame();

    void EndTurn();
    void ChooseCharacterCard(int nr, std::string name);

    // Actions
    void CurrentPlayerGetGold();
    void CurrentPlayerDrawsCard();
    void CurrentPlayerUsesCharacterPower();
    void CurrentPlayerBuildsBuilding(int nr);

    infra::Subject eventSubject;

    models::BuildingCard DrawCardFromStack();
    models::Player GetCurrentPlayer();
    bool CharacterHasPlayer(int nr);

  private:
    void NextTurn();
    void NextRound();
    void StartRound();

    std::vector<std::pair<std::string, bool>> choosingTurns;
}; // class GameController
} // namespace domain
} // namespace mach