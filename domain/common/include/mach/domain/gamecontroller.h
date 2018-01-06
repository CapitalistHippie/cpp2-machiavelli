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

    void CurrentPlayerGetGold();
    void CurrentPlayerDrawsCard();

    void EndTurn();

    infra::Subject eventSubject;

  private:
    models::BuildingCard DrawCardFromStack();
}; // class GameController
} // namespace domain
} // namespace mach
