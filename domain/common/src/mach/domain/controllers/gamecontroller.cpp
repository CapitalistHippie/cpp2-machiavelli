#include "mach\domain\controllers\gamecontroller.h"
#include "mach\domain\events\gameendedevent.h"
#include "mach\domain\events\gamestartedevent.h"

using namespace mach::domain::events;

GameController::GameController(Game game)
  : game{ game }
{
    game.isStarted = false;
}

GameController::~GameController()
{
}

void GameController::AddPlayer(std::string playerName)
{
    if (!game.isStarted && std::find(game.playersWaiting.begin(), game.playersWaiting.end(), playerName) !=
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
    auto it = std::find(game.playersWaiting.begin(), game.playersWaiting.end(), playerName);
    if (it != game.playersWaiting.end())
    {
        game.playersWaiting.erase(it);
    }
}

void GameController::StartGame()
{
    game = Game();
    // TODO load stack and characters from file
    for (auto& playerName : game.playersWaiting)
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
    game.isStarted = true;
    auto evt = GameStartedEvent();
    evt.game = game;
    eventSubject.NotifyObservers(evt);
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
