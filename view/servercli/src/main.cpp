// Memory leak detection.
#ifndef NDEBUG
#define _CRTDBG_MAP_ALLOC
#define _CRTDBG_MAP_ALLOC_NEW
#include <stdlib.h>

#include <crtdbg.h>
#endif // #ifndef NDEBUG

#include <iostream>
#include <memory>

#include <mach/infra/threadpool.h>

#include <mach/app/server.h>

#include "mach/view/servercli.h"

using namespace mach;
using namespace mach::view;

int main()
{
#ifndef NDEBUG
    // Memory leak detection.
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // #ifndef NDEBUG

    int exitCode = 0;

#ifdef NDEBUG
    try
    {
#endif
        infra::ThreadPool threadPool(2);

        app::Server server(threadPool);

        ServerCli serverCli(server, std::cin, std::cout);
        serverCli.Start();

        server.Stop();
        threadPool.StopThreads();
#ifdef NDEBUG
    }
    catch (const std::system_error& e)
    {
        exitCode = e.code().value();

        std::cerr << "A fatal error occurred: " << exitCode << ": " << e.what() << "\nPress any key to exit.";
        std::cin.ignore();
    }
    catch (const std::exception& e)
    {
        exitCode = -1;

        std::cerr << "A fatal error occurred: " << e.what() << "\nPress any key to exit.";
        std::cin.ignore();
    }
    catch (...)
    {
        exitCode = -1;

        std::cerr << "An unknown fatal error occurred.\nPress any key to exit.";
        std::cin.ignore();
    }
#endif

    return exitCode;
}
