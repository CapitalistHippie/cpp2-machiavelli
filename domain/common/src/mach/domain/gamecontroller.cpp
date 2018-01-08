#include "mach/domain/gamecontroller.h"
#include "mach/domain/characterpowerhelper.h"
#include "mach/domain/events/gameendedevent.h"
#include "mach/domain/events/gamestartedevent.h"
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
        game.playersWaiting.push_back("the other");
        // throw std::exception("Duplicate player name!");
    }
}

void GameController::RemovePlayer(std::string playerName)
{
    if (game.state != GameState::Waiting)
    {
        throw std::exception("Can not remove player: game has already started");
    }
    auto it = std::find(game.playersWaiting.begin(), game.playersWaiting.end(), playerName);
    if (it != game.playersWaiting.end())
    {
        game.playersWaiting.erase(it);
    }
}

void GameController::StartGame()
{
    if (game.state != GameState::Waiting)
    {
        throw std::exception("Game is already started");
    }
    auto playersWaiting = game.playersWaiting;
    game = Game();

    game.characters = characterCardRepository.GetCards();
    game.buildingCardStack.clear();
    for (auto& card : buildingCardRepository.GetCards())
    {
        game.buildingCardStack.push_back(std::move(card));
    }

    for (auto& playerName : playersWaiting)
    {
        auto newPlayer = Player(playerName);
        // Draw 4 cards
        for (int i = 0; i < 4; i++)
        {
            newPlayer.hand.push_back(DrawCardFromStack());
        }
        newPlayer.gold = 2;
        game.players.push_back(newPlayer);
    }
    game.king = game.players[0];
    game.state = GameState::Running;
    NextRound();

    auto evt = GameStartedEvent();
    evt.game = game;
    eventSubject.NotifyObservers(evt);
}

void GameController::NextTurn()
{
    if (game.state != GameState::Running || game.state != GameState::FinalRound)
    {
        throw std::exception("Can not do next turn: Game is in illegal state");
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
    else if (!CharacterHasPlayer(game.characterHasTurn))
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
        throw std::exception("Can not start next round: game is in illegal state");
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
        std::uniform_int_distribution<int> dist(0, game.charactersToChooseFrom.size() - 1);
        int i = dist(rd);
        // TODO Make random again
        // game.charactersToChooseFrom.erase(game.charactersToChooseFrom.begin() + dist(rd));
        game.charactersToChooseFrom.erase(game.charactersToChooseFrom.begin());

        // Initialize choosing turns for picking Character cards
        choosingTurns.clear();
        auto findResult = std::find_if(
          game.players.begin(), game.players.end(), [=](Player player) { return player.name != game.king.name; });

        auto otherPlayer = findResult->name;

        // King chooses one and keeps it
        choosingTurns.push_back(std::pair<std::string, bool>(game.king.name, true));
        // Other player chooses one and discards one
        choosingTurns.push_back(std::pair<std::string, bool>(otherPlayer, true));
        choosingTurns.push_back(std::pair<std::string, bool>(otherPlayer, false));
        // King chooses one and discards one
        choosingTurns.push_back(std::pair<std::string, bool>(game.king.name, true));
        choosingTurns.push_back(std::pair<std::string, bool>(game.king.name, false));
        // Other player chooses one and discards one
        choosingTurns.push_back(std::pair<std::string, bool>(otherPlayer, true));
        choosingTurns.push_back(std::pair<std::string, bool>(otherPlayer, false));

        game.state = GameState::ChoosingCharacters;

        auto evt = NextRoundEvent();
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
        throw std::exception("Can not end game: game is in illegal state");
    }
    game.state = GameState::Ended;
    auto evt = GameEndedEvent();
    evt.game = game;

    Player winner;
    for (auto p : game.players)
    {
        if (p.GetPoints() > winner.GetPoints() ||
            p.GetPoints() == winner.GetPoints() && p.GetPointsFromBuildings() > winner.GetPointsFromBuildings())
        {
            winner = p;
        }
    }
    evt.winner = winner.name;
    eventSubject.NotifyObservers(evt);
}

void GameController::EndTurn()
{
    if (game.state != GameState::Running)
    {
        throw std::exception("Can not end turn: game is in illegal state");
    }
    else
    {
        NextTurn();
    }
}

void GameController::ChooseCharacterCard(int nr, std::string name)
{
    if (game.state != GameState::ChoosingCharacters)
    {
        throw std::exception("Players aren't choosing character cards");
    }
    // else if (choosingTurns[0].first != name)
    //{
    //    throw std::exception("It's not your turn to choose");
    //}
    else
    {
        auto findResult = std::find_if(game.charactersToChooseFrom.begin(),
                                       game.charactersToChooseFrom.end(),
                                       [nr](dal::models::CharacterCard card) { return card.number == nr; });
        if (findResult == game.charactersToChooseFrom.end())
        {
            throw std::exception("You can not choose this card");
        }
        else
        {
            dal::models::CharacterCard character = *findResult;
            if (choosingTurns[0].second)
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
            choosingTurns.erase(choosingTurns.begin());
            if (choosingTurns.size() == 0)
            {
                StartRound();
            }
        }
    }
}

void mach::domain::GameController::CurrentPlayerGetGold()
{
    if (game.playerReceivedGoldOrCards)
    {
        throw std::exception("You already did that this round");
    }
    else
    {
        game.playerReceivedGoldOrCards = true;
        Player p = GetCurrentPlayer();
        p.gold += 2;
    }
}

void mach::domain::GameController::CurrentPlayerDrawsCard()
{
    if (game.playerReceivedGoldOrCards)
    {
        throw std::exception("You already did that this round");
    }
    else
    {
        // TODO
    }
}

void mach::domain::GameController::CurrentPlayerUsesCharacterPower()
{
    if (game.playerUsedCharacterPower)
    {
        throw std::exception("You already used your character power");
    }
    else
    {
        CharacterPowerHelper helper;
        helper.UseCharacterPower(game.characterHasTurn, *this);
    }
}

void mach::domain::GameController::CurrentPlayerBuildsBuilding(unsigned int nr)
{
    Player currentPlayer = GetCurrentPlayer();
    if (currentPlayer.hand.size() > nr || nr < 0)
    {
        throw std::exception("Invalid building selected");
    }
    else
    {
        dal::models::BuildingCard chosenCard = currentPlayer.hand[nr];
        if (chosenCard.cost > currentPlayer.gold)
        {
            throw std::exception("You cannot afford that");
        }
        else
        {
            currentPlayer.hand.erase(
              std::find_if(currentPlayer.hand.begin(), currentPlayer.hand.end(), [=](dal::models::BuildingCard card) {
                  return card.name == chosenCard.name;
              }));
            currentPlayer.gold -= chosenCard.cost;
            currentPlayer.buildings.push_back(chosenCard);
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

models::Player mach::domain::GameController::GetCurrentPlayer()
{
    auto findRes = std::find_if(game.players.begin(), game.players.end(), [=](Player player) {
        auto res = std::find_if(player.characters.begin(),
                                player.characters.end(),
                                [=](dal::models::CharacterCard card) { return card.number == game.characterHasTurn; });
        return res != player.characters.end();
    });
    return *findRes;
}

bool mach::domain::GameController::CharacterHasPlayer(int nr)
{
    auto findRes = std::find_if(game.players.begin(), game.players.end(), [nr](Player player) {
        auto res = std::find_if(player.characters.begin(),
                                player.characters.end(),
                                [nr](dal::models::CharacterCard card) { return card.number == nr; });
        return res != player.characters.end();
    });
    return findRes != game.players.end();
}
