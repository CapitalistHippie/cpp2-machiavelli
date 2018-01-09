#include "mach/domain/characterpowerhelper.h"
#include <algorithm>

using namespace mach::domain;
using namespace mach::domain::models;

mach::domain::CharacterPowerHelper::CharacterPowerHelper()
{
}

void mach::domain::CharacterPowerHelper::UseCharacterPower(int nr, GameController& gameController)
{
    if (!gameController.game.CharacterHasPlayer(gameController.game.characterHasTurn))
    {
        return;
    }
    Player currentPlayer = gameController.game.GetCurrentPlayer();
    switch (nr)
    {
        case 1:
            DoAssassin(currentPlayer, gameController);
            break;
        case 2:
            DoThief(currentPlayer, gameController);
            break;
        case 3:
            DoMagician(currentPlayer, gameController);
            break;
        case 4:
            DoKing(currentPlayer, gameController);
            break;
        case 5:
            DoBishop(currentPlayer, gameController);
            break;
        case 6:
            DoMerchant(currentPlayer, gameController);
            break;
        case 7:
            DoArchitect(currentPlayer, gameController);
            break;
        case 8:
            DoWarlord(currentPlayer, gameController);
            break;
    }
}

void mach::domain::CharacterPowerHelper::DoAssassin(models::Player currentPlayer, GameController gameController)
{
    // TODO Choose
    int nr = 0;
    gameController.game.killedCharacter = nr;
}

void mach::domain::CharacterPowerHelper::DoThief(Player currentPlayer, GameController gameController)
{
    auto tempVec = gameController.game.players;
    tempVec.erase(
      std::find_if(tempVec.begin(), tempVec.end(), [=](Player player) { return player.name == currentPlayer.name; }));
    Player chosenPlayer = tempVec[0];
    currentPlayer.gold += chosenPlayer.gold;
    chosenPlayer.gold = 0;
}

void mach::domain::CharacterPowerHelper::DoMagician(models::Player currentPlayer, GameController gameController)
{
    // TODO choose:
    if (true)
    {
        // 1. Swap hands with another player
        auto tempVec = gameController.game.players;
        tempVec.erase(std::find_if(
          tempVec.begin(), tempVec.end(), [=](Player player) { return player.name == currentPlayer.name; }));
        Player chosenPlayer = tempVec[0];
        auto tempHand = currentPlayer.hand;
        currentPlayer.hand = chosenPlayer.hand;
        chosenPlayer.hand = tempHand;
    }
    else
    {
        // 2. Swap any number of cards for others from the stack
        // TODO
    }
}

void mach::domain::CharacterPowerHelper::DoKing(models::Player currentPlayer, GameController gameController)
{
    currentPlayer.gold += currentPlayer.GetAmountOfBuildingsByColor(dal::models::BuildingColor::Yellow);
}

void mach::domain::CharacterPowerHelper::DoBishop(models::Player currentPlayer, GameController gameController)
{
    currentPlayer.gold += currentPlayer.GetAmountOfBuildingsByColor(dal::models::BuildingColor::Blue);
}

void mach::domain::CharacterPowerHelper::DoMerchant(models::Player currentPlayer, GameController gameController)
{
    currentPlayer.gold += 1 + currentPlayer.GetAmountOfBuildingsByColor(dal::models::BuildingColor::Green);
}

void mach::domain::CharacterPowerHelper::DoArchitect(models::Player currentPlayer, GameController gameController)
{
    // Draw 2 cards
    currentPlayer.hand.push_back(gameController.DrawCardFromStack());
    currentPlayer.hand.push_back(gameController.DrawCardFromStack());
}

void mach::domain::CharacterPowerHelper::DoWarlord(models::Player currentPlayer, GameController gameController)
{
    auto tempVec = gameController.game.players;
    tempVec.erase(std::find_if(
      tempVec.begin(), tempVec.end(), [&](const Player& player) { return player.name == currentPlayer.name; }));
    Player otherPlayer = tempVec[0];
    dal::models::BuildingCard chosenBuilding = otherPlayer.buildings[0];
    if (chosenBuilding.cost > 1)
    {
        if (currentPlayer.gold < (chosenBuilding.cost - 1))
        {
            throw std::exception("You cannot afford that");
        }
        else
        {
            currentPlayer.gold -= (chosenBuilding.cost - 1);
        }
    }
    otherPlayer.buildings.erase(std::find_if(
      otherPlayer.buildings.begin(), otherPlayer.buildings.end(), [&](const dal::models::BuildingCard& card) {
          return card.name == chosenBuilding.name;
      }));
    currentPlayer.gold += 1 + currentPlayer.GetAmountOfBuildingsByColor(dal::models::BuildingColor::Red);
}
