#include "mach/view/console.h"

#include <stdlib.h>

void mach::view::ClearConsole()
{
#ifdef _WIN32
    system("cls");
#else
    for (int i = 0; i < 100; ++i)
    {
        std::cout << '\n';
    }
#endif
}
