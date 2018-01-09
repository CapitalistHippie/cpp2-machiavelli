#include "mach/view/statehandlers/choosingcharacters.h"

#include <mach/domain/events/characterchosenevent.h>
#include <mach/domain/events/nextturnevent.h>
#include <mach/view/console.h>

using namespace mach;
using namespace mach::view::statehandlers;

void ChoosingCharacters::EnterState()
{
    ClearConsole();

    // Commands
    RegisterCommand<int>("choose");

    RegisterCommandObserver([](const infra::CliCommand& command) { return command.name == "choose"; },
                            std::bind(&ChoosingCharacters::ChooseCharacterCommandHandler, this, std::placeholders::_1));

    if (context.recentGameState.choosingTurns.size() > 0 &&
        context.recentGameState.choosingTurns[0].first == client.GetConfiguration().playerName)
    {
        PrintOptions(context.recentGameState);
    }
    else
    {
        outputStream << "Wait for your turn to choose... \n";
    }

    // Events
    RegisterClientObserver<domain::events::CharacterChosenEvent>([&](const domain::events::CharacterChosenEvent& evt) {
        ClearConsole();
        if (evt.game.choosingTurns.size() > 0 &&
            evt.game.choosingTurns[0].first == client.GetConfiguration().playerName)
        {
            PrintOptions(evt.game);
        }
        else
        {
            outputStream << "Wait for your turn to choose... \n";
        }
    });

    RegisterClientObserver<domain::events::NextTurnEvent>([&](const domain::events::NextTurnEvent& evt) {
        context.recentGameState = evt.game;
        context.SetState(ClientCliState::PlayingRound);
    });
}

void mach::view::statehandlers::ChoosingCharacters::ChooseCharacterCommandHandler(const infra::CliCommand& command)
{
    auto number = *std::static_pointer_cast<int>(command.parameters[0]);

    client.SendChooseCharacterCommand(number);
}

void mach::view::statehandlers::ChoosingCharacters::PrintOptions(domain::models::Game game)
{
    outputStream << "It's your turn to choose! \n";
    outputStream << "Which card do you want to ";
    if (game.choosingTurns[0].second)
    {
        outputStream << "pick?\n";
    }
    else
    {
        outputStream << "discard?\n";
    }
    for (auto card : game.charactersToChooseFrom)
    {
        outputStream << card.number << ": " << card.name << "\n";
    }
    outputStream << "Commands: \n choose <nr>: choose a character card\n";
}
