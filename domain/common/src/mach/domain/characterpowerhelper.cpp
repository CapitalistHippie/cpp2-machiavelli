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
    std::vector<std::pair<std::string, int>> vec;
    for (auto it = gameController.game.characters.begin() + 1; it != gameController.game.characters.end(); it++)
    {
        vec.push_back(std::make_pair(it->name, it->number));
    }

    evt.choices = vec;

    gameController.eventSubject.NotifyObservers(evt);

    gameController.game.state = GameState::AwaitingPlayerChoice;
    gameController.doWhenPlayerChooses = [&, currentPlayer, evt](std::vector<int> numbers) {
        int nr = numbers[0] + 1;
        if (nr < 2 || nr > 8)
        {
            gameController.eventSubject.NotifyObservers(evt);
        }
        else
        {
            gameController.game.killedCharacter = nr;
            gameController.game.state = GameState::Running;
            auto evt = events::GameUpdatedEvent();
            evt.game = gameController.game;
            evt.message = std::string("Player ") + currentPlayer.name + " selected character to kill!";

            gameController.eventSubject.NotifyObservers(evt);
        }
    };
}

void mach::domain::CharacterPowerHelper::DoThief(models::Player& currentPlayer, GameController& gameController)
{
    auto otherPlayer = std::find_if(gameController.game.players.begin(),
                                    gameController.game.players.end(),
                                    [&](const Player& player) { return player.name != currentPlayer.name; });

    currentPlayer.gold += otherPlayer->gold;
    otherPlayer->gold = 0;

    auto evt = events::GameUpdatedEvent();
    evt.game = gameController.game;
    evt.message = std::string("Player ") + currentPlayer.name + " stole gold from " + otherPlayer->name + "!";

    gameController.eventSubject.NotifyObservers(evt);
}

void mach::domain::CharacterPowerHelper::DoMagician(models::Player& currentPlayer, GameController& gameController)
{
    auto evt = domain::events::IntChoiceNecessaryEvent();
    std::vector<std::pair<std::string, int>> vec{ std::make_pair("Swap hands", 1), std::make_pair("Redraw cards", 2) };

    evt.choices = vec;

    gameController.eventSubject.NotifyObservers(evt);

    gameController.game.state = GameState::AwaitingPlayerChoice;
    gameController.doWhenPlayerChooses = [&, evt](std::vector<int> numbers) {
        int nr = numbers[0] + 1;
        if (nr == 1)
        {
            // 1. Swap hands with another player
            auto otherPlayer = std::find_if(gameController.game.players.begin(),
                                            gameController.game.players.end(),
                                            [&](Player player) { return player.name != currentPlayer.name; });
            auto tempHand = std::move(currentPlayer.hand);
            currentPlayer.hand = std::move(otherPlayer->hand);
            otherPlayer->hand = std::move(tempHand);

            gameController.game.state = GameState::Running;
            auto evt = events::GameUpdatedEvent();
            evt.game = gameController.game;
            evt.message =
              std::string("Player ") + currentPlayer.name + " swapped hands with " + otherPlayer->name + "!";

            gameController.eventSubject.NotifyObservers(evt);
        }
        else if (nr == 2)
        {
            // 2. Swap any number of cards for others from the stack
            gameController.game.state = GameState::AwaitingPlayerChoice;

            auto evt = domain::events::CardChoiceNecessaryEvent();
            evt.choices = currentPlayer.hand;
            gameController.eventSubject.NotifyObservers(evt);

            auto choices = currentPlayer.hand;

            gameController.doWhenPlayerChooses = [&, choices, evt](std::vector<int> numbers) {
                for (int& number : numbers)
                {
                    if (choices.size() <= number || number < 0)
                    {
                        gameController.eventSubject.NotifyObservers(evt);
                        return;
                    }
                }
                std::sort(numbers.begin(), numbers.end());
                for (auto it = numbers.rbegin(); it != numbers.rend(); it++)
                {
                    currentPlayer.hand.erase(currentPlayer.hand.begin() + *it);
                }
                for (int number : numbers)
                {
                    currentPlayer.hand.push_back(gameController.DrawCardFromStack());
                }

                gameController.game.state = GameState::Running;
                auto evt = events::GameUpdatedEvent();
                evt.game = gameController.game;
                evt.message = std::string("Player ") + currentPlayer.name + " discarded and drew " +
                              std::to_string(numbers.size()) + " new cards !";

                gameController.eventSubject.NotifyObservers(evt);
            };
        }
        else
        {
            // Incorrect value: redo it
            gameController.eventSubject.NotifyObservers(evt);
        }
    };
}

void mach::domain::CharacterPowerHelper::DoKing(models::Player& currentPlayer, GameController& gameController)
{
    int amount = currentPlayer.GetAmountOfBuildingsByColor(dal::models::BuildingColor::Yellow);

    auto evt = events::GameUpdatedEvent();
    evt.game = gameController.game;
    evt.message = std::string("Player ") + currentPlayer.name + " got " + std::to_string(amount) + " gold!";
    gameController.eventSubject.NotifyObservers(evt);

    currentPlayer.gold += amount;
}

void mach::domain::CharacterPowerHelper::DoBishop(models::Player& currentPlayer, GameController& gameController)
{
    int amount = currentPlayer.GetAmountOfBuildingsByColor(dal::models::BuildingColor::Blue);

    auto evt = events::GameUpdatedEvent();
    evt.game = gameController.game;
    evt.message = std::string("Player ") + currentPlayer.name + " got " + std::to_string(amount) + " gold!";
    gameController.eventSubject.NotifyObservers(evt);

    currentPlayer.gold += amount;
}

void mach::domain::CharacterPowerHelper::DoMerchant(models::Player& currentPlayer, GameController& gameController)
{
    int amount = 1 + currentPlayer.GetAmountOfBuildingsByColor(dal::models::BuildingColor::Green);

    auto evt = events::GameUpdatedEvent();
    evt.game = gameController.game;
    evt.message = std::string("Player ") + currentPlayer.name + " got " + std::to_string(amount) + " gold!";
    gameController.eventSubject.NotifyObservers(evt);

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
    gameController.eventSubject.NotifyObservers(evt);
}

void mach::domain::CharacterPowerHelper::DoWarlord(models::Player& currentPlayer, GameController& gameController)
{
    auto otherPlayer = std::find_if(gameController.game.players.begin(),
                                    gameController.game.players.end(),
                                    [&](const Player& player) { return player.name != currentPlayer.name; });

    // Warlord gets amount of red buildings of gold.
    int amount = currentPlayer.GetAmountOfBuildingsByColor(dal::models::BuildingColor::Red);
    currentPlayer.gold += amount;

    auto otherPlayerBuildingCount = otherPlayer->buildings.size();

    // Check of ie het goedkoopst te slopen gebouw kan betalen.
    bool canAfford = std::any_of(
      otherPlayer->buildings.begin(), otherPlayer->buildings.end(), [&](const dal::models::BuildingCard& building) {
          if (building.cost == 1 || (building.cost - 1) <= currentPlayer.gold)
          {
              return true;
          }
          return false;
      });

    // Hij mag geen gebouwen verwijderen uit steden die al uit 8 of meer gebouwen bestaan.
    if (otherPlayerBuildingCount == 0 || otherPlayerBuildingCount >= 8 || !canAfford)
    {
        auto evt = events::GameUpdatedEvent();
        evt.game = gameController.game;
        evt.message = std::string("Player ") + currentPlayer.name + " got " + std::to_string(amount) + " gold!";

        gameController.eventSubject.NotifyObservers(evt);
    }
    else
    {
        auto evt = domain::events::CardChoiceNecessaryEvent();
        evt.choices = otherPlayer->buildings;
        gameController.eventSubject.NotifyObservers(evt);

        auto choices = evt.choices;

        gameController.game.state = GameState::AwaitingPlayerChoice;
        gameController.doWhenPlayerChooses = [&, otherPlayer, choices](std::vector<int> numbers) {
            int nr = numbers[0];

            if (nr < 0 || nr >= choices.size())
            {
                auto evt = domain::events::CardChoiceNecessaryEvent();
                evt.choices = otherPlayer->buildings;
                gameController.eventSubject.NotifyObservers(evt);
            }
            else
            {
                dal::models::BuildingCard chosenBuilding = otherPlayer->buildings[nr];

                if (chosenBuilding.cost > 1)
                {
                    if (currentPlayer.gold < (chosenBuilding.cost - 1))
                    {
                        throw std::exception("You cannot afford that.");
                    }
                    else
                    {
                        currentPlayer.gold -= (chosenBuilding.cost - 1);
                    }
                }

                otherPlayer->buildings.erase(std::find_if(
                  otherPlayer->buildings.begin(),
                  otherPlayer->buildings.end(),
                  [&](const dal::models::BuildingCard& card) { return card.name == chosenBuilding.name; }));

                gameController.game.state = GameState::Running;
                auto evt = events::GameUpdatedEvent();
                evt.game = gameController.game;
                evt.message = std::string("Player ") + currentPlayer.name + " destroyed a " + chosenBuilding.name +
                              " from " + otherPlayer->name + "!";

                gameController.eventSubject.NotifyObservers(evt);
            }
        };
    }
}
