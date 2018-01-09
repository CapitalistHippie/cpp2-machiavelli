#include "mach/view/statehandlers/waitforgametostart.h"

#include <mach/domain/events/clientconnectedevent.h>
#include <mach/domain/events/nextturnevent.h>
#include <mach/view/console.h>

using namespace mach;
using namespace mach::view::statehandlers;

void WaitForGameToStart::EnterState()
{
    ClearConsole();

    RegisterClientObserver<domain::events::ClientConnectedEvent>([&](const domain::events::ClientConnectedEvent& evt) {
        outputStream << "Client with id '" << evt.clientInfo.id << "' connected from '" << evt.clientInfo.source
                     << "'.\n";
    });

    RegisterClientObserver<domain::events::CharacterChosenEvent>([&](const domain::events::CharacterChosenEvent& evt) {
        context.recentGameState = evt.game;
        context.SetState(ClientCliState::ChoosingCharacters);
    });

    RegisterClientObserver<domain::events::NextTurnEvent>([&](const domain::events::NextTurnEvent& evt) {
        context.recentGameState = evt.game;
        context.SetState(ClientCliState::PlayingRound);
    });

    outputStream << "Waiting for game to start... \n";
}
