#include "mach/view/statehandlers/playinground.h"

#include <mach/domain/events/gameupdatedevent.h>
#include <mach/domain/events/nextturnevent.h>
#include <mach/view/console.h>
#include <mach/view/helpers.h>

#include <string>

using namespace mach;
using namespace mach::view::statehandlers;

void PlayingRound::EnterState()
{
    ClearConsole();

    // Commands
    RegisterCommand("gold");
    RegisterCommand("card");
    RegisterCommand<int>("build");
    RegisterCommand("power");
    RegisterCommand("end");
    RegisterCommand<std::string>("choose");

    RegisterCommandObserver([](const infra::CliCommand& command) { return command.name == "gold"; },
                            std::bind(&PlayingRound::GetGoldCommandHandler, this));

    RegisterCommandObserver([](const infra::CliCommand& command) { return command.name == "card"; },
                            std::bind(&PlayingRound::GetCardCommandHandler, this));

    RegisterCommandObserver([](const infra::CliCommand& command) { return command.name == "build"; },
                            std::bind(&PlayingRound::BuildBuildingCommandHandler, this, std::placeholders::_1));

    RegisterCommandObserver([](const infra::CliCommand& command) { return command.name == "power"; },
                            std::bind(&PlayingRound::UseCharacterPowerCommandHandler, this));

    RegisterCommandObserver([](const infra::CliCommand& command) { return command.name == "end"; },
                            std::bind(&PlayingRound::EndTurnCommandHandler, this));

    RegisterCommandObserver([](const infra::CliCommand& command) { return command.name == "choose"; },
                            std::bind(&PlayingRound::ChooseCardsCommandHandler, this, std::placeholders::_1));

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

    RegisterClientObserver<domain::events::CardChoiceNecessaryEvent>(
      [&](const domain::events::CardChoiceNecessaryEvent& evt) {
          if (myTurn)
          {
              ClearConsole();
              outputStream << "Please choose: \n";
              for (int i = 0; i < evt.choices.size(); i++)
              {
                  auto card = evt.choices[i];
                  outputStream << "[" << i + 1 << "]: " << card.name << ": " << card.cost << ": "
                               << BuildingColorEnumToString(card.color) << ": " << card.description << "\n";
              }
              outputStream << "\nAvailable commands: \n";
              outputStream << "choose <nr>: choose a card \n\n";
          }
      });

    RegisterClientObserver<domain::events::IntChoiceNecessaryEvent>(
      [&](const domain::events::IntChoiceNecessaryEvent& evt) {
          if (myTurn)
          {
              ClearConsole();
              outputStream << "Please choose: \n";
              for (int i = 0; i < evt.choices.size(); i++)
              {
                  auto card = evt.choices[i];
                  outputStream << i << "\n";
              }
              outputStream << "\nAvailable commands: \n";
              outputStream << "choose <nr>: choose an option \n\n";
          }
      });

    RegisterClientObserver<domain::events::IllegalActionEvent>(
      [&](const domain::events::IllegalActionEvent& evt) { outputStream << evt.message << "\n\n"; });

    RegisterClientObserver<domain::events::NextTurnEvent>([&](const domain::events::NextTurnEvent& evt) {
        myTurn = evt.game.GetCurrentPlayerName() == client.GetConfiguration().playerName;
        ClearConsole();
        PrintGameStatus(evt.game);
    });

    RegisterClientObserver<domain::events::GameEndedEvent>([&](const domain::events::GameEndedEvent& evt) {
        context.recentGameState = evt.game;
        context.SetState(ClientCliState::Ended);
    });
}

void mach::view::statehandlers::PlayingRound::GetGoldCommandHandler()
{
    if (myTurn)
    {
        client.SendGetGoldCommand();
    }
}

void mach::view::statehandlers::PlayingRound::GetCardCommandHandler()
{
    if (myTurn)
    {
        client.SendGetCardCommand();
    }
}

void mach::view::statehandlers::PlayingRound::UseCharacterPowerCommandHandler()
{
    if (myTurn)
    {
        client.SendUseCharacterPowerCommand();
    }
}

void mach::view::statehandlers::PlayingRound::BuildBuildingCommandHandler(const infra::CliCommand& command)
{
    if (myTurn)
    {
        auto number = *std::static_pointer_cast<int>(command.parameters[0]);

        client.SendBuildBuildingCommand(number - 1);
    }
}

void mach::view::statehandlers::PlayingRound::ChooseCardsCommandHandler(const infra::CliCommand& command)
{
    if (myTurn)
    {
        auto choicesString = *std::static_pointer_cast<std::string>(command.parameters[0]);
        std::vector<int> choices;

        std::stringstream choicesStringstream(choicesString);

        while (!choicesStringstream.eof())
        {
            int choice;
            choicesStringstream >> choice;
            choice--;

            choices.push_back(choice);
        }

        client.SendChooseCommand(std::move(choices));
    }
}

void mach::view::statehandlers::PlayingRound::EndTurnCommandHandler()
{
    if (myTurn)
    {
        client.SendEndTurnCommand();
    }
}

void mach::view::statehandlers::PlayingRound::PrintGameStatus(domain::models::Game game)
{
    const auto currentCharacter =
      std::find_if(game.characters.begin(), game.characters.end(), [&](const dal::models::CharacterCard& card) {
          return card.number == game.characterHasTurn;
      });

    outputStream << "Current character: " << currentCharacter->name << "\n";
    outputStream << "Current player: " << game.GetCurrentPlayerName() << "\n";
    outputStream << "Got cards/gold this turn? " << BooleanToString(game.playerReceivedGoldOrCards) << " | ";
    outputStream << "Character power used? " << BooleanToString(game.playerUsedCharacterPower) << "\n";
    for (auto player : game.players)
    {
        outputStream << "----" << player.name << "----\n";
        outputStream << player.name << "\n";
        outputStream << "- Gold: " << player.gold << "\n";
        outputStream << "- Buildings: \n";
        for (auto b : player.buildings)
        {
            outputStream << "-- " << b.name << ": " << b.cost << ": " << BuildingColorEnumToString(b.color) << ": "
                         << b.description << "\n";
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
            outputStream << "[" << i + 1 << "] " << card.name << ". Cost: " << card.cost
                         << ". Color: " << BuildingColorEnumToString(card.color) << "\n";
        }
        outputStream << "\nAvailable commands: \n";
        outputStream << "- gold: get 2 gold \n";
        outputStream << "- card: draw a card \n";
        outputStream << "- build <nr>: Builds a building from your hand \n";
        outputStream << "- power: use your character power \n";
        outputStream << "- end: end your turn \n\n";
    }
}
