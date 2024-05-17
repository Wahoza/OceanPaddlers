//Author: Radomir Dinic, modified by Kilian Kolb & Ema Selmanovic
//Description: MainState for the Game. Note: Camera is not generated from TileMap.
//License: LICENSE.txt
#pragma once

#include "GameObjectManager.hpp"
#include "GameState.hpp"
#include "SpriteManager.hpp"
#include "PhysicsManager.hpp"
#include "SoundManager.hpp"
#include "Hud.hpp"
#include "IEvent.hpp"
#include "LevelGenerator.hpp"

namespace mmt_gd
{
class FINALFRONTIER_API MainState final : public GameState
{
public:
    MainState(GameStateManager* gameStateManager, Game* game);

    void init() override;
    void exit() override;
    void update(float deltaTime) override;
    void draw() override;

    void ListenforEnd(IEvent::Ptr event);

private:
    SpriteManager     m_spriteManager;
    GameObjectManager m_gameObjectManager;
    SoundManager      m_soundManager;
    PhysicsManager*   m_PhysicsManager;
    Hud*              m_Hud;
    LevelGenerator    m_gen;

    bool paused = false;
};
} // namespace mmt_gd
