#pragma once

#include "GameObjectManager.hpp"
#include "GameState.hpp"
#include "Hud.hpp"
#include "IEvent.hpp"
#include "PhysicsManager.hpp"
#include "SoundManager.hpp"
#include "SpriteManager.hpp"
namespace mmt_gd
{
class FINALFRONTIER_API MainLevelState final : public GameState
{
public:
    MainLevelState(GameStateManager* gameStateManager, Game* game);

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
    Hud              m_Hud;

    bool paused = false;
    bool isActive = false;
};
} // namespace mmt_gd
