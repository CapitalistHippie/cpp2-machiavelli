#include "mach/view/statehandlers/playinground.h"

#include <mach/domain/events/gameupdatedevent.h>
#include <mach/domain/events/nextturnevent.h>
#include <mach/view/console.h>

#include <string>

using namespace mach;
using namespace mach::view::statehandlers;

void PlayingRound::EnterState()
{
    ClearConsole();

    // Commands
    commandParser.RegisterCommand("gold");
    commandParser.RegisterCommand("card");
    commandParser.RegisterCommand<int>("build");
    commandParser.RegisterCommand("power");
    commandParser.RegisterCommand("end");

    commandSubject.RegisterObserver<infra::CliCommand>(
      [](const infra::CliCommand& command) { return command.name == "gold"; },
      std::bind(&PlayingRound::GetGoldCommandHandler, this));

    commandSubject.RegisterObserver<infra::CliCommand>(
      [](const infra::CliCommand& command) { return command.name == "card"; },
      std::bind(&PlayingRound::GetCardCommandHandler, this));

    commandSubject.RegisterObserver<infra::CliCommand>(
      [](const infra::CliCommand& command) { return command.name == "build"; },
      std::bind(&PlayingRound::BuildBuildingCommandHandler, this, std::placeholders::_1));

    commandSubject.RegisterObserver<infra::CliCommand>(
      [](const infra::CliCommand& command) { return command.name == "power"; },
      std::bind(&PlayingRound::UseCharacterPowerCommandHandler, this));

    commandSubject.RegisterObserver<infra::CliCommand>(
      [](const infra::CliCommand& command) { return command.name == "end"; },
      std::bind(&PlayingRound::EndTurnCommandHandler, this));

    outputStream << context.recentGameState.GetCurrentPlayerName() << "\n\n";
    outputStream << client.GetConfiguration().playerName << "\n\n";
    outputStream << (context.recentGameState.GetCurrentPlayerName() == client.GetConfiguration().playerName) << "\n\n";

    myTurn = context.recentGameState.GetCurrentPlayerName() == client.GetConfiguration().playerName;
    PrintGameStatus(context.recentGameState);

    RegisterClientObserver<domain::events::GameUpdatedEvent>([&](const domain::events::GameUpdatedEvent& evt) {
        ClearConsole();
        outputStream << evt.message << "\n\n\n";
        PrintGameStatus(evt.game);
    });

    RegisterClientObserver<domain::events::ChoiceNecessaryEvent>([&](const domain::events::ChoiceNecessaryEvent& evt) {
        if (myTurn)
        {
            ClearConsole();
        }
    });

    RegisterClientObserver<domain::events::IllegalActionEvent>(
      [&](const domain::events::IllegalActionEvent& evt) { outputStream << evt.message << "\n\n"; });

    RegisterClientObserver<domain::events::NextTurnEvent>([&](const domain::events::NextTurnEvent& evt) {
        myTurn = evt.game.GetCurrentPlayerName() == client.GetConfiguration().playerName;
        ClearConsole();
        PrintGameStatus(evt.game);
    });
}

void mach::view::statehandlers::PlayingRound::GetGoldCommandHandler()
{
    client.SendGetGoldCommand();
}

void mach::view::statehandlers::PlayingRound::GetCardCommandHandler()
{
    client.SendGetCardCommand();
}

void mach::view::statehandlers::PlayingRound::UseCharacterPowerCommandHandler()
{
    client.SendUseCharacterPowerCommand();
}

void mach::view::statehandlers::PlayingRound::BuildBuildingCommandHandler(const infra::CliCommand& command)
{
    auto number = *std::static_pointer_cast<int>(command.parameters[0]);

    client.SendBuildBuildingCommand(number);
}

void mach::view::statehandlers::PlayingRound::EndTurnCommandHandler()
{
    client.SendEndTurnCommand();
}

void mach::view::statehandlers::PlayingRound::PrintGameStatus(domain::models::Game game)
{
    outputStream << "Current character: " << game.characterHasTurn << "\n";
    outputStream << "Current player: " << game.GetCurrentPlayerName() << "\n";
    outputStream << "Got cards/gold this turn? " << game.playerReceivedGoldOrCards << " | ";
    outputStream << "Character power used? " << game.playerUsedCharacterPower << "\n";
    for (auto player : game.players)
    {
        outputStream << "----" << player.name << "----\n";
        outputStream << player.name << "\n";
        outputStream << "- Gold: " << player.gold << "\n";
        outputStream << "- Buildings: \n";
        for (auto b : player.buildings)
        {
            outputStream << "-- " << b.name << ": " << b.cost << ": " << b.color << ": " << b.description << "\n";
        }
        outputStream << "\n";
    }

    if (!myTurn)
    {
        outputStream << "\nPlease await your turn. \n";
    }
    else
    {
        outputStream << "\nIt is your turn! \n";
        outputStream << "Your hand: \n";
        for (int i = 0; i < game.GetCurrentPlayer().hand.size(); i++)
        {
            auto card = game.GetCurrentPlayer().hand[i];
            outputStream << "[" << i + 1 << "] " << card.name << ". Cost: " << card.cost << ". Color: " << card.color
                         << "\n";
        }
        outputStream << "\nAvailable commands: \n";
        outputStream << "- gold: get 2 gold \n";
        outputStream << "- card: draw a card \n";
        outputStream << "- build <nr>: Builds a building from your hand \n";
        outputStream << "- power: use your character power \n";
        outputStream << "- end: end your turn \n\n";
    }
}
