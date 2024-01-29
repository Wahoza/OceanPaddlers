#include "stdafx.h"

#include "Game.hpp"


int main()
{
#if _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    mmt_gd::Game game;
    game.run();

    return 0;
}
