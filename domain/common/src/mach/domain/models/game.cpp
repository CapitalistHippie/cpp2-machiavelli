#include "mach/domain/models/game.h"

using namespace mach::domain::models;

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
