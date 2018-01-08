#include "mach/domain/gamecontroller.h"
#include "mach/domain/characterpowerhelper.h"
#include "mach/domain/events/gameendedevent.h"
#include "mach/domain/events/gamestartedevent.h"
#include "mach/domain/events/nextroundevent.h"
#include "mach/domain/events/nextturnevent.h"
#include <algorithm>
#include <random>

using namespace mach::domain;
using namespace mach::domain::events;
using namespace mach::domain::models;

GameController::GameController()
{
    game.isStarted = false;
}

GameController::GameController(Game game)
  : game{ game }
{
    game.isStarted = false;
}

void GameController::AddPlayer(std::string playerName)
{
    if (!game.isStarted && std::find(game.playersWaiting.begin(), game.playersWaiting.end(), playerName) ==
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
    auto it = std::find(game.playersWaiting.begin(), game.playersWaiting.end(), playerName);
    if (it != game.playersWaiting.end())
    {
        game.playersWaiting.erase(it);
    }
}

void GameController::StartGame()
{
    auto playersWaiting = game.playersWaiting;
    game = Game();

    // TODO load stack and characters from file
    // TODO remove hardcoded loading
    for (int i = 0; i < 100; i++)
    {
        game.buildingCardStack.push_back(BuildingCard(i, "Card nr:" + std::to_string(i), BuildingColor::Blue, ""));
    }
    for (int i = 0; i < 8; i++)
    {
        game.characters.push_back(CharacterCard(i, "Character nr: " + std::to_string(i)));
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
    game.isStarted = true;
    NextRound();

    auto evt = GameStartedEvent();
    evt.game = game;
    eventSubject.NotifyObservers(evt);
}

void GameController::NextTurn()
{
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
    if (game.isFinalRound)
    {
        EndGame();
        return;
    }
    for (auto player : game.players)
    {
        // Find the new king
        auto findResult = std::find_if(
          player.characters.begin(), player.characters.end(), [](CharacterCard card) { return card.nr == 4; });
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

    game.playersChoosingCharacterCards = true;

    auto evt = NextRoundEvent();
    evt.game = game;

    eventSubject.NotifyObservers(evt);
}

void mach::domain::GameController::StartRound()
{
    game.playersChoosingCharacterCards = false;
    game.characterHasTurn = 0;
    NextTurn();
}

void GameController::EndGame()
{
    game.isStarted = false;
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
    if (game.playersChoosingCharacterCards)
    {
        throw std::exception("Players are still choosing cards: there is no turn to end");
    }
    else
    {
        NextTurn();
    }
}

void GameController::ChooseCharacterCard(int nr, std::string name)
{
    if (!game.playersChoosingCharacterCards)
    {
        throw std::exception("Players aren't choosing cards");
    }
    else if (choosingTurns[0].first != name)
    {
        throw std::exception("It's not your turn to choose");
    }
    else
    {
        auto findResult = std::find_if(game.charactersToChooseFrom.begin(),
                                       game.charactersToChooseFrom.end(),
                                       [nr](CharacterCard card) { return card.nr == nr; });
        if (findResult == game.charactersToChooseFrom.end())
        {
            throw std::exception("You can not choose this card");
        }
        else
        {
            CharacterCard character = *findResult;
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
                             [=](CharacterCard card) { return card.nr == character.nr; }),
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

void mach::domain::GameController::CurrentPlayerBuildsBuilding(int nr)
{
    Player currentPlayer = GetCurrentPlayer();
    if (currentPlayer.hand.size() > nr || nr < 0)
    {
        throw std::exception("Invalid building selected");
    }
    else
    {
        BuildingCard chosenCard = currentPlayer.hand[nr];
        if (chosenCard.cost > currentPlayer.gold)
        {
            throw std::exception("You cannot afford that");
        }
        else
        {
            currentPlayer.hand.erase(std::find(currentPlayer.hand.begin(), currentPlayer.hand.end(), chosenCard));
            currentPlayer.gold -= chosenCard.cost;
            currentPlayer.buildings.push_back(chosenCard);
        };
    }
}

BuildingCard GameController::DrawCardFromStack()
{
    if (!game.buildingCardStack.empty())
    {
        BuildingCard card = *game.buildingCardStack.begin();
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
        auto res = std::find_if(player.characters.begin(), player.characters.end(), [=](CharacterCard card) {
            return card.nr == game.characterHasTurn;
        });
        return res != player.characters.end();
    });
    return *findRes;
}

bool mach::domain::GameController::CharacterHasPlayer(int nr)
{
    auto findRes = std::find_if(game.players.begin(), game.players.end(), [nr](Player player) {
        auto res = std::find_if(
          player.characters.begin(), player.characters.end(), [nr](CharacterCard card) { return card.nr == nr; });
        return res != player.characters.end();
    });
    return findRes != game.players.end();
}
