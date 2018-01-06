#pragma once
#include "mach\domain\models\game.h"
#include "mach\domain\models\player.h"
#include <mach/infra/subject.h>
#include <memory>
#include <stack>
#include <string>
#include <vector>

class GameController
{
  public:
    GameController(Game game);
    ~GameController();

    Game game;

    void AddPlayer(std::string player);
    void RemovePlayer(std::string playerName);

    void StartGame();
    void EndGame();

    void CurrentPlayerGetGold();
    void CurrentPlayerDrawsCard();

    void EndTurn();

    mach::infra::Subject eventSubject;

  private:
    BuildingCard DrawCardFromStack();
};
