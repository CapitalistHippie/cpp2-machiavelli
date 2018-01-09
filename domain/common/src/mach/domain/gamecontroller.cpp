#include "mach/domain/gamecontroller.h"
#include "mach/domain/characterpowerhelper.h"
#include "mach/domain/events/characterchosenevent.h"
#include "mach/domain/events/choicenecessaryevent.h"
#include "mach/domain/events/gameendedevent.h"
#include "mach/domain/events/gamestartedevent.h"
#include "mach/domain/events/gameupdatedevent.h"
#include "mach/domain/events/nextroundevent.h"
#include "mach/domain/events/nextturnevent.h"
#include <algorithm>
#include <random>

using namespace mach;
using namespace mach::domain;
using namespace mach::domain::events;
using namespace mach::domain::models;

GameController::GameController()

{
    game.state = GameState::Waiting;
}

GameController::GameController(Game game)
  : game{ game }
{
    game.state = GameState::Waiting;
}

void GameController::AddPlayer(std::string playerName)
{
    if (game.state == GameState::Waiting &&
        std::find(game.playersWaiting.begin(), game.playersWaiting.end(), playerName) ==
          game.playersWaiting.end()) // Player name is not already present
    {
        game.playersWaiting.push_back(playerName);
    }
    else
    {
        throw std::exception("Duplicate player name!");
    }
}

void GameController::RemovePlayer(std::string playerName)
{
    if (game.state != GameState::Waiting)
    {
        throw std::exception("Can not remove player. Game has already started.");
    }
    auto it = std::find(game.playersWaiting.begin(), game.playersWaiting.end(), playerName);
    if (it != game.playersWaiting.end())
    {
        game.playersWaiting.erase(it);
    }
}

void GameController::StartGame(bool skip)
{
    if (game.state != GameState::Waiting)
    {
        throw std::exception("Game is already started.");
    }
    if (skip)
    {
    }
    auto playersWaiting = game.playersWaiting;
    game = Game();

    game.characters = characterCardRepository.GetCards();
    game.buildingCardStack.clear();
    for (auto& card : buildingCardRepository.GetCards())
    {
        game.buildingCardStack.push_back(std::move(card));
    }

    auto rng = std::default_random_engine{};
    std::shuffle(game.buildingCardStack.begin(), game.buildingCardStack.end(), rng);

    int counter = 0;

    for (auto& playerName : playersWaiting)
    {
        auto newPlayer = Player(playerName);
        // Draw 4 cards
        for (int i = 0; i < 4; i++)
        {
            newPlayer.hand.push_back(DrawCardFromStack());
        }
        newPlayer.gold = 2;

        if (skip)
        {
            newPlayer.characters.push_back(game.characters[counter]);
            newPlayer.characters.push_back(game.characters[counter + 1]);
            counter += 3;
        }

        game.players.push_back(newPlayer);
    }
    game.king = game.players[0];
    game.state = GameState::Running;

    if (skip)
    {
        StartRound();
    }
    else
    {
        NextRound();
    }
}

void GameController::NextTurn()
{
    if (game.state != GameState::Running && game.state != GameState::FinalRound)
    {
        throw std::exception("Can not do next turn. Game is in illegal state.");
    }
    game.characterHasTurn++;
    if (game.killedCharacter == game.characterHasTurn)
    {
        game.characterHasTurn++;
    }
    if (game.characterHasTurn == 9)
    {
        NextRound();
    }
    else if (!game.CharacterHasPlayer(game.characterHasTurn))
    {
        NextTurn();
    }
    else
    {
        // Reset variables
        game.buildingsStillAllowedToPlayThisTurn = 1;
        if (game.characterHasTurn == 7)
        {
            // Bouwmeester mag 3 gebouwen plaatsen
            game.buildingsStillAllowedToPlayThisTurn += 2;
        }
        game.playerReceivedGoldOrCards = false;
        game.playerUsedCharacterPower = false;

        auto evt = NextTurnEvent();
        evt.game = game;

        eventSubject.NotifyObservers(evt);
    }
}

void GameController::NextRound()
{
    if (game.state == GameState::FinalRound)
    {
        EndGame();
        return;
    }
    else if (game.state != GameState::Running)
    {
        throw std::exception("Can not start next round. Game is in illegal state.");
    }
    else
    {
        for (auto player : game.players)
        {
            // Find the new king
            auto findResult = std::find_if(player.characters.begin(),
                                           player.characters.end(),
                                           [](dal::models::CharacterCard card) { return card.number == 4; });
            if (findResult != player.characters.end())
            {
                game.king = player;
            }
            player.characters.clear();
        }
        game.killedCharacter = 0;
        game.characterHasTurn = 1;

        // Shuffle character cards and remove a random one
        game.charactersToChooseFrom = game.characters;

        std::random_device rd;
        std::uniform_int_distribution<int> dist(0, game.charactersToChooseFrom.size());

        game.charactersToChooseFrom.erase(game.charactersToChooseFrom.begin() + dist(rd));

        // Initialize choosing turns for picking Character cards
        game.choosingTurns.clear();
        auto findResult = std::find_if(
          game.players.begin(), game.players.end(), [=](Player player) { return player.name != game.king.name; });

        auto otherPlayer = findResult->name;

        // King chooses one and keeps it
        game.choosingTurns.push_back(std::pair<std::string, bool>(game.king.name, true));
        // Other player chooses one and discards one
        game.choosingTurns.push_back(std::pair<std::string, bool>(otherPlayer, true));
        game.choosingTurns.push_back(std::pair<std::string, bool>(otherPlayer, false));
        // King chooses one and discards one
        game.choosingTurns.push_back(std::pair<std::string, bool>(game.king.name, true));
        game.choosingTurns.push_back(std::pair<std::string, bool>(game.king.name, false));
        // Other player chooses one and discards one
        game.choosingTurns.push_back(std::pair<std::string, bool>(otherPlayer, true));
        game.choosingTurns.push_back(std::pair<std::string, bool>(otherPlayer, false));

        game.state = GameState::ChoosingCharacters;

        auto evt = CharacterChosenEvent();
        evt.game = game;

        eventSubject.NotifyObservers(evt);
    }
}

void mach::domain::GameController::StartRound()
{
    game.state = GameState::Running;
    game.characterHasTurn = 0;
    NextTurn();
}

void GameController::EndGame()
{
    if (game.state != GameState::FinalRound)
    {
        throw std::exception("Can not end game: game is in illegal state.");
    }
    game.state = GameState::Ended;
    auto evt = GameEndedEvent();
    evt.game = game;

    const auto& winner = game.GetWinner();
    evt.winner = winner.name;
    eventSubject.NotifyObservers(evt);
}

void mach::domain::GameController::MakeChoice(int nr)
{
    if (game.state == GameState::AwaitingPlayerChoice)
    {
        doWhenPlayerChooses(nr);
    }
    else
    {
        throw std::exception("Invalid command.");
    }
}

void GameController::EndTurn()
{
    if (game.state != GameState::Running && game.state != GameState::FinalRound)
    {
        throw std::exception("Can not end turn: game is in illegal state.");
    }
    else
    {
        NextTurn();
    }
}

void GameController::ChooseCharacterCard(int nr)
{
    if (game.state != GameState::ChoosingCharacters)
    {
        throw std::exception("Players aren't choosing character cards.");
    }
    else
    {
        std::string name = game.choosingTurns[0].first;
        auto findResult = std::find_if(game.charactersToChooseFrom.begin(),
                                       game.charactersToChooseFrom.end(),
                                       [nr](dal::models::CharacterCard card) { return card.number == nr; });
        if (findResult == game.charactersToChooseFrom.end())
        {
            throw std::exception("You can not choose this card.");
        }
        else
        {
            dal::models::CharacterCard character = *findResult;
            if (game.choosingTurns[0].second)
            {
                // Keep the card
                auto player = std::find_if(
                  game.players.begin(), game.players.end(), [name](Player player) { return player.name == name; });
                player->characters.push_back(character);
            }
            // Discard
            game.charactersToChooseFrom.erase(
              std::remove_if(game.charactersToChooseFrom.begin(),
                             game.charactersToChooseFrom.end(),
                             [=](dal::models::CharacterCard card) { return card.number == character.number; }),
              game.charactersToChooseFrom.end());
            game.choosingTurns.erase(game.choosingTurns.begin());
            if (game.choosingTurns.size() == 0)
            {
                StartRound();
            }
            else
            {
                auto evt = CharacterChosenEvent();
                evt.game = game;

                eventSubject.NotifyObservers(evt);
            }
        }
    }
}

void mach::domain::GameController::CurrentPlayerGetGold()
{
    if (game.playerReceivedGoldOrCards)
    {
        throw std::exception("You already picked a card or received gold this round.");
    }
    else
    {
        game.playerReceivedGoldOrCards = true;
        Player& p = game.GetCurrentPlayer();
        p.gold += 2;

        auto evt = GameUpdatedEvent();
        evt.game = game;
        evt.message = "Player received 2 gold!";

        eventSubject.NotifyObservers(evt);
    }
}

void mach::domain::GameController::CurrentPlayerDrawsCard()
{
    if (game.playerReceivedGoldOrCards)
    {
        throw std::exception("You already picked a card or received gold this round.");
    }
    else
    {
        game.state = GameState::AwaitingPlayerChoice;

        choices.clear();
        choices.push_back(DrawCardFromStack());
        choices.push_back(DrawCardFromStack());

        auto evt = ChoiceNecessaryEvent();
        evt.choices = choices;

        eventSubject.NotifyObservers(evt);

        doWhenPlayerChooses = [&](int nr) {
            if (nr >= choices.size() || nr < 0)
            {
                // Incorrect value
                auto evt = ChoiceNecessaryEvent();
                evt.choices = choices;

                eventSubject.NotifyObservers(evt);
            }
            else
            {
                // Correct value
                auto cardChosen = choices.at(nr);
                game.GetCurrentPlayer().hand.push_back(std::move(cardChosen));

                game.state = GameState::Running;

                auto evt = GameUpdatedEvent();
                evt.game = game;
                evt.message = "Player picked a card!";

                eventSubject.NotifyObservers(evt);
            }
        };
    }
}

void mach::domain::GameController::CurrentPlayerUsesCharacterPower()
{
    if (game.playerUsedCharacterPower)
    {
        throw std::exception("You already used your character power.");
    }
    else
    {
        CharacterPowerHelper helper;
        helper.UseCharacterPower(game.characterHasTurn, *this);

        auto evt = GameUpdatedEvent();
        evt.game = game;
        evt.message = "Player used character power!";

        eventSubject.NotifyObservers(evt);
    }
}

void mach::domain::GameController::CurrentPlayerBuildsBuilding(unsigned int nr)
{
    auto currentPlayer = std::find_if(game.players.begin(), game.players.end(), [=](Player player) {
        auto res = std::find_if(player.characters.begin(),
                                player.characters.end(),
                                [=](dal::models::CharacterCard card) { return card.number == game.characterHasTurn; });
        return res != player.characters.end();
    });
    if (currentPlayer == game.players.end())
    {
        throw std::exception("Invalid building selected.");
    }
    if (currentPlayer->hand.size() < nr || nr < 0)
    {
        throw std::exception("Invalid building selected.");
    }
    else
    {
        dal::models::BuildingCard chosenCard = currentPlayer->hand[nr];
        if (chosenCard.cost == 1000) // currentPlayer.gold)
        {
            throw std::exception("You cannot afford that.");
        }
        else
        {
            currentPlayer->hand.erase(
              std::find_if(currentPlayer->hand.begin(), currentPlayer->hand.end(), [=](dal::models::BuildingCard card) {
                  return card.name == chosenCard.name;
              }));
            currentPlayer->gold -= chosenCard.cost;
            currentPlayer->buildings.push_back(chosenCard);

            int i = currentPlayer->buildings.size();
            if (currentPlayer->buildings.size() != 1000) // >= 8) TODO put back
            {
                game.state = GameState::FinalRound;
            }

            auto evt = GameUpdatedEvent();
            evt.game = game;
            evt.message = currentPlayer->name + " has built a " + chosenCard.name;

            eventSubject.NotifyObservers(evt);
        };
    }
}

dal::models::BuildingCard GameController::DrawCardFromStack()
{
    if (!game.buildingCardStack.empty())
    {
        dal::models::BuildingCard card = *game.buildingCardStack.begin();
        game.buildingCardStack.pop_front();
        return card;
    }
    else
    {
        throw std::exception("Stack is empty!");
    }
}
