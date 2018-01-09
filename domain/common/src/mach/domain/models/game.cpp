#include "mach/domain/models/game.h"

using namespace mach::domain::models;

Player& Game::GetCurrentPlayer()
{
    return *std::find_if(players.begin(), players.end(), [=](Player player) {
        auto res = std::find_if(player.characters.begin(),
                                player.characters.end(),
                                [=](dal::models::CharacterCard card) { return card.number == characterHasTurn; });
        return res != player.characters.end();
    });
}

std::string Game::GetCurrentPlayerName() const
{
    Player player;
    for (Player p : players)
    {
        for (dal::models::CharacterCard card : p.characters)
        {
            if (card.number == characterHasTurn)
            {
                player = p;
            }
        }
    }
    return player.name;
}

bool Game::CharacterHasPlayer(int nr)
{
    auto findRes = std::find_if(players.begin(), players.end(), [nr](Player player) {
        auto res = std::find_if(player.characters.begin(),
                                player.characters.end(),
                                [nr](dal::models::CharacterCard card) { return card.number == nr; });
        return res != player.characters.end();
    });
    return findRes != players.end();
}

Player& mach::domain::models::Game::GetWinner()
{
    Player* winner = &players[0];

    for (unsigned int i = 1; i < players.size(); ++i)
    {
        auto& player = players[i];

        if (player.GetPoints() > winner->GetPoints() ||
            player.GetPoints() == winner->GetPoints() &&
              player.GetPointsFromBuildings() > winner->GetPointsFromBuildings())
        {
            winner = &player;
        }
    }

    return *winner;
}

std::ostream& operator<<(std::ostream& os, const Game& game)
{
    return os << game.state << '|' << game.characterHasTurn << '|' << game.playerReceivedGoldOrCards << '|'
              << game.playerUsedCharacterPower << '|' << game.buildingsStillAllowedToPlayThisTurn << '|'
              << game.killedCharacter << '|' << game.king << '|' << game.characters << '|' << game.buildingCardStack
              << '|' << game.players << '|' << game.playersWaiting << '|' << game.charactersToChooseFrom << '|'
              << game.choosingTurns;
}

std::istream& operator>>(std::istream& is, Game& game)
{
    is >> game.state;
    is.ignore();
    is >> game.characterHasTurn;
    is.ignore();
    is >> game.playerReceivedGoldOrCards;
    is.ignore();
    is >> game.playerUsedCharacterPower;
    is.ignore();
    is >> game.buildingsStillAllowedToPlayThisTurn;
    is.ignore();
    is >> game.killedCharacter;
    is.ignore();
    is >> game.king;
    is.ignore();
    is >> game.characters;
    is.ignore();
    is >> game.buildingCardStack;
    is.ignore();
    is >> game.players;
    is.ignore();
    is >> game.playersWaiting;
    is.ignore();
    is >> game.charactersToChooseFrom;
    is.ignore();
    is >> game.choosingTurns;
    return is;
}
