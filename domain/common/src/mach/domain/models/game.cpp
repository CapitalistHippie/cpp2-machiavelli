#include "mach/domain/models/game.h"

using namespace mach::domain::models;

std::ostream& operator<<(std::ostream& os, const Game& game)
{
    return os << game.isStarted << '|' << game.characterHasTurn << '|' << game.king << '|' << game.characters << '|'
              << game.buildingCardStack << '|' << game.players << '|' << game.playersWaiting;
}

std::istream& operator>>(std::istream& is, Game& game)
{
    is >> game.isStarted;
    is.ignore();
    is >> game.characterHasTurn;
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
    return is;
}
