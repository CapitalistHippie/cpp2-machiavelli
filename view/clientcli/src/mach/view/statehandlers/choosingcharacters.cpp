#include "mach/view/statehandlers/choosingcharacters.h"

#include <mach/domain/events/characterchosenevent.h>
#include <mach/view/console.h>

using namespace mach;
using namespace mach::view::statehandlers;

void ChoosingCharacters::EnterState()
{
    ClearConsole();

    if (context.recentGameState.choosingTurns[0].first == client.GetConfiguration().playerName)
    {
        outputStream << "It's your turn to choose! \n";
        outputStream << "Which card do you want to ";
        if (context.recentGameState.choosingTurns[0].second)
        {
            outputStream << "pick?\n";
        }
        else
        {
            outputStream << "discard?\n";
        }
        for (auto card : context.recentGameState.charactersToChooseFrom)
        {
            outputStream << card.number << ": " << card.name << "\n";
        }
    }
    else
    {
        outputStream << "Wait for your turn to choose... \n";
    }

    RegisterClientObserver<domain::events::CharacterChosenEvent>([&](const domain::events::CharacterChosenEvent& evt) {
        if (evt.game.choosingTurns[0].first == client.GetConfiguration().playerName)
        {
            outputStream << "It's your turn to choose! \n";
            outputStream << "Which card do you want to ";
            if (evt.game.choosingTurns[0].second)
            {
                outputStream << "pick?\n";
            }
            else
            {
                outputStream << "discard?\n";
            }
        }
        else
        {
            outputStream << "Wait for your turn to choose... \n";
        }
    });

    RegisterClientObserver<domain::events::ClientConnectedEvent>(
      [&](const domain::events::ClientConnectedEvent& evt) { outputStream << "NOH :( \n"; });

    RegisterClientObserver<domain::events::GameStartedEvent>(
      [&](const domain::events::GameStartedEvent& evt) { outputStream << "YESH \n"; });

    outputStream << "Choosing characters... \n";
}
