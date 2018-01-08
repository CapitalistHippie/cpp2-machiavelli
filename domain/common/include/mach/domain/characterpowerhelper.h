#pragma once
#include "mach/domain/gamecontroller.h"
#include "mach/domain/models/player.h"
#include <unordered_map>

namespace mach
{
namespace domain
{
class CharacterPowerHelper
{
    /* std::unordered_map<int,
                        void (mach::domain::CharacterPowerHelper::*)(models::Player player,
                                                                     GameController gameController)>
       map;*/

  public:
    CharacterPowerHelper();
    void UseCharacterPower(int nr, GameController gameController);

    void DoAssassin(models::Player currentPlayer, GameController gameController);
    void DoThief(models::Player currentPlayer, GameController gameController);
    void DoMagician(models::Player currentPlayer, GameController gameController);
    void DoKing(models::Player currentPlayer, GameController gameController);
    void DoBishop(models::Player currentPlayer, GameController gameController);
    void DoMerchant(models::Player currentPlayer, GameController gameController);
    void DoArchitect(models::Player currentPlayer, GameController gameController);
    void DoWarlord(models::Player currentPlayer, GameController gameController);

}; // class CharacterPowerHelper
} // namespace domain
} // namespace mach
