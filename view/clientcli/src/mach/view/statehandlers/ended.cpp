#include "mach/view/statehandlers/ended.h"

#include <mach/domain/events/characterchosenevent.h>
#include <mach/view/console.h>

using namespace mach;
using namespace mach::view::statehandlers;

void Ended::EnterState()
{
    ClearConsole();

    PrintGameStatus(context.recentGameState);

    outputStream << "Game has ended! \n";
}

void mach::view::statehandlers::Ended::PrintGameStatus(domain::models::Game game)
{
    for (auto player : game.players)
    {
        outputStream << "----" << player.name << "----\n";
        outputStream << player.name << "\n";
        outputStream << "Points : " + player.GetPoints() << "\n";
        outputStream << "- Buildings: \n";
        for (auto b : player.buildings)
        {
            outputStream << "-- " << b.name << ": " << b.cost << ": " << b.color << ": " << b.description << "\n";
        }
        outputStream << "\n";
    }
}