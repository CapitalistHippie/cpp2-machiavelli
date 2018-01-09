#include "mach/domain/characterpowerhelper.h"
#include "mach/domain/events/cardchoicenecessaryevent.h"
#include "mach/domain/events/gameupdatedevent.h"
#include "mach/domain/events/intchoicenecessaryevent.h"
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
    Player& currentPlayer = gameController.game.GetCurrentPlayer();
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

void mach::domain::CharacterPowerHelper::DoAssassin(models::Player& currentPlayer, GameController& gameController)
{

    auto evt = domain::events::IntChoiceNecessaryEvent();
    std::vector<int> vec;
    for (int i = 1; i < 9; i++)
    {
        vec.push_back(i);
    }

    evt.choices = vec;

    gameController.eventSubject.NotifyObservers(evt);

    gameController.game.state = GameState::AwaitingPlayerChoice;
    gameController.doWhenPlayerChooses = [&, currentPlayer](std::vector<int> numbers) {
        int nr = numbers[0];
        if (nr < 2 || nr > 8)
        {
            gameController.game.killedCharacter = nr;
            gameController.game.state = GameState::Running;
            auto evt = events::GameUpdatedEvent();
            evt.game = gameController.game;
            evt.message = std::string("Player ") + currentPlayer.name + " selected character to kill!";

            gameController.eventSubject.NotifyObservers(evt);
        }
        else
        {
            gameController.eventSubject.NotifyObservers(evt);
        }
    };
}

void mach::domain::CharacterPowerHelper::DoThief(models::Player& currentPlayer, GameController& gameController)
{
    auto otherPlayer = std::find_if(gameController.game.players.begin(),
                                    gameController.game.players.end(),
                                    [&](Player player) { return player.name != currentPlayer.name; });
    currentPlayer.gold += otherPlayer->gold;
    otherPlayer->gold = 0;

    auto evt = events::GameUpdatedEvent();
    evt.game = gameController.game;
    evt.message = std::string("Player ") + currentPlayer.name + " stole gold from " + otherPlayer->name + "!";

    gameController.eventSubject.NotifyObservers(evt);
}

void mach::domain::CharacterPowerHelper::DoMagician(models::Player& currentPlayer, GameController& gameController)
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

        auto evt = events::GameUpdatedEvent();
        evt.game = gameController.game;
        evt.message = std::string("Player ") + currentPlayer.name + " swapped hands with " + chosenPlayer.name + "!";

        gameController.eventSubject.NotifyObservers(evt);
    }
    else
    {
        // 2. Swap any number of cards for others from the stack
        // TODO
    }
}

void mach::domain::CharacterPowerHelper::DoKing(models::Player& currentPlayer, GameController& gameController)
{
    int amount = currentPlayer.GetAmountOfBuildingsByColor(dal::models::BuildingColor::Yellow);

    auto evt = events::GameUpdatedEvent();
    evt.game = gameController.game;
    evt.message = std::string("Player ") + currentPlayer.name + " got " + std::to_string(amount) + " gold!";

    currentPlayer.gold += amount;
}

void mach::domain::CharacterPowerHelper::DoBishop(models::Player& currentPlayer, GameController& gameController)
{
    int amount = currentPlayer.GetAmountOfBuildingsByColor(dal::models::BuildingColor::Blue);

    auto evt = events::GameUpdatedEvent();
    evt.game = gameController.game;
    evt.message = std::string("Player ") + currentPlayer.name + " got " + std::to_string(amount) + " gold!";

    currentPlayer.gold += amount;
}

void mach::domain::CharacterPowerHelper::DoMerchant(models::Player& currentPlayer, GameController& gameController)
{
    int amount = 1 + currentPlayer.GetAmountOfBuildingsByColor(dal::models::BuildingColor::Green);

    auto evt = events::GameUpdatedEvent();
    evt.game = gameController.game;
    evt.message = std::string("Player ") + currentPlayer.name + " got " + std::to_string(amount) + " gold!";

    currentPlayer.gold += amount;
}

void mach::domain::CharacterPowerHelper::DoArchitect(models::Player& currentPlayer, GameController& gameController)
{
    // Draw 2 cards
    currentPlayer.hand.push_back(gameController.DrawCardFromStack());
    currentPlayer.hand.push_back(gameController.DrawCardFromStack());

    auto evt = events::GameUpdatedEvent();
    evt.game = gameController.game;
    evt.message = std::string("Player ") + currentPlayer.name + " drew 2 cards!";
}

void mach::domain::CharacterPowerHelper::DoWarlord(models::Player& currentPlayer, GameController& gameController)
{
    auto tempVec = gameController.game.players;
    tempVec.erase(std::find_if(
      tempVec.begin(), tempVec.end(), [&](const Player& player) { return player.name == currentPlayer.name; }));
    Player otherPlayer = tempVec[0];

    if (otherPlayer.buildings.size() == 0)
    {
        throw std::exception("There are no buildings for the warlord to destory!");
    }

    auto evt = domain::events::CardChoiceNecessaryEvent();
    evt.choices = otherPlayer.buildings;
    gameController.eventSubject.NotifyObservers(evt);

    auto choices = evt.choices;

    gameController.game.state = GameState::AwaitingPlayerChoice;
    gameController.doWhenPlayerChooses = [&](std::vector<int> numbers) {
        int nr = numbers[0];
        if (choices.size() > nr || nr < 0)
        {
            auto evt = domain::events::CardChoiceNecessaryEvent();
            evt.choices = otherPlayer.buildings;
            gameController.eventSubject.NotifyObservers(evt);
        }
        else
        {
            dal::models::BuildingCard chosenBuilding = otherPlayer.buildings[nr];
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

            gameController.game.state = GameState::Running;
            auto evt = events::GameUpdatedEvent();
            evt.game = gameController.game;
            evt.message = std::string("Player ") + currentPlayer.name + " destroyed a " + chosenBuilding.name +
                          " from " + otherPlayer.name + "!";
        }
    };
}
