#pragma once

#include "GameObjectManager.hpp"
#include "GameState.hpp"
#include "SpriteManager.hpp"
#include "PhysicsManager.hpp"
#include "SoundManager.hpp"
#include "Hud.hpp"
#include "IEvent.hpp"
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
};
} // namespace mmt_gd
