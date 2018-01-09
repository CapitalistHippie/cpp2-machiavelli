#include "mach/view/statehandlers/ended.h"

#include <mach/domain/events/characterchosenevent.h>
#include <mach/view/console.h>
#include <mach/view/helpers.h>

using namespace mach;
using namespace mach::view::statehandlers;

void Ended::EnterState()
{
    ClearConsole();

    if (context.recentGameState.GetWinner().name == client.GetConfiguration().playerName)
    {
        outputStream << "Game has ended! You have WON!!!!\n\n";
    }
    else
    {
        outputStream << "Game has ended! You have scored a second place!\n\n";
    }

    PrintGameStatus(context.recentGameState);

    outputStream << "\n";
}

void mach::view::statehandlers::Ended::PrintGameStatus(const domain::models::Game& game) const
{
    for (const auto& player : game.players)
    {
        outputStream << "----" << player.name << "----\n";
        outputStream << "- Points: " << player.GetPoints() << ".\n";
        outputStream << "- Buildings:\n";
        for (auto b : player.buildings)
        {
            outputStream << "-- " << b.name << ": " << b.cost << ": " << BuildingColorEnumToString(b.color) << ": "
                         << b.description << "\n";
        }
        outputStream << "\n";
    }
}
