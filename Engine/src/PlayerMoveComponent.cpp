#include "stdafx.h"

#include "PlayerMoveComponent.hpp"

#include "GameObject.hpp"
#include "InputManager.hpp"
#include "RigidBodyComponent.hpp"
namespace mmt_gd
{
PlayerMoveComponent::PlayerMoveComponent(GameObject& gameObject,  const int playerIndex, bool rigidControll) :
IComponent(gameObject),
m_playerIndex(playerIndex),
rigidControll(rigidControll)
{
}

bool PlayerMoveComponent::init()
{
    return true;
}

void PlayerMoveComponent::update(const float deltaTime)
{
    const auto   speed = 1'000.0F; // pixels/second
    sf::Vector2f translation{};

        if (InputManager::getInstance().isKeyDown("right", m_playerIndex))
        {
            translation.x += 1;
        }
        if (InputManager::getInstance().isKeyDown("left", m_playerIndex))
        {
            translation.x -= 1;
        }
        if (InputManager::getInstance().isKeyDown("up", m_playerIndex))
        {
            translation.y -= 1;
        }
        if (InputManager::getInstance().isKeyDown("down", m_playerIndex))
        {
            translation.y += 1;
        }

    Vector2f moveVec = translation;
    moveVec = moveVec / sqrt(moveVec.x * moveVec.x + moveVec.y * moveVec.y);

    if (m_gameObject.getComponent<TetherComponent>() != nullptr)
    {
            Vector2f Y = m_gameObject.getComponent<TetherComponent>().get()->orientation;
            float    dotProduct    = moveVec.x * Y.x + moveVec.y * Y.y;
            float    lengthSquared = Y.x * Y.x + Y.y * Y.y;

            float scalarProjection = dotProduct / lengthSquared;

            sf::Vector2f X_projected(scalarProjection * Y.x, scalarProjection * Y.y);

            moveVec = X_projected;
    }
    if (m_gameObject.getComponent<RigidBodyComponent>() != nullptr && rigidControll)
    {
        m_gameObject.getComponent<RigidBodyComponent>()->addVelocity(translation * 100.f * deltaTime);
    }
    else
    {
        //note: still needs to be normalized
        m_gameObject.move(translation * speed * deltaTime);
    }


}
} // namespace mmt_gd
