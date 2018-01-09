#pragma once

#include <memory>
#include <stack>
#include <string>
#include <vector>

#include <mach/infra/subject.h>

#include "mach/domain/models/game.h"
#include "mach/domain/models/player.h"

#include "mach/dal/models/buildingcard.h"
#include "mach/dal/models/charactercard.h"

#include "mach/dal/buildingcardrepository.h"
#include "mach/dal/charactercardrepository.h"

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
    void CurrentPlayerBuildsBuilding(unsigned int nr);

    infra::Subject eventSubject;

    mach::dal::models::BuildingCard DrawCardFromStack();
    models::Player GetCurrentPlayer();
    bool CharacterHasPlayer(int nr);

  private:
    mach::dal::BuildingCardRepository buildingCardRepository;
    mach::dal::CharacterCardRepository characterCardRepository;

    void NextTurn();
    void NextRound();
    void StartRound();
}; // class GameController
} // namespace domain
} // namespace mach
