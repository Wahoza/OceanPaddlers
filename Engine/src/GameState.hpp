#pragma once

#include "FinalFrontier/dll_export.hpp"

#include <memory>

namespace mmt_gd
{
class GameStateManager;
class Game;

class FINALFRONTIER_API GameState
{
public:
    using Ptr = std::shared_ptr<GameState>;

    GameState(GameStateManager* gameStateManager, Game* game) : m_gameStateManager(gameStateManager), m_game(game)
    {
    }

    virtual ~GameState()                    = default;
    GameState(const GameState&)             = delete;
    GameState(const GameState&&)            = delete;
    GameState& operator=(const GameState&)  = delete;
    GameState& operator=(const GameState&&) = delete;

    virtual void init()
    {
    }

    virtual void exit()
    {
    }

    virtual void update(float delta) = 0;
    virtual void draw()              = 0;

protected:
    GameStateManager* m_gameStateManager;
    Game*             m_game;
};
} // namespace mmt_gd
