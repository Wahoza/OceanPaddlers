#include "stdafx.h"

#include "PlayerMoveComponent.hpp"

#include "GameObject.hpp"
#include "InputManager.hpp"
#include "RigidBodyComponent.hpp"
#include "SpriteRenderComponent.hpp"
#include <cmath>

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
sf::Vector2i PlayerMoveComponent::toSpriteOffset(int value, int playerIdx)
{
    if (playerIdx == 0)
    {
        return (Vector2i{value, 0});
    }
    if (playerIdx == 1)
    {
        return (Vector2i{0, value});
    }
}

void PlayerMoveComponent::update(const float deltaTime)
{
    const auto   speed = 10.0F; // pixels/second
    sf::Vector2f translation{};
    Vector2i     SpriteOffset = {0, 0};
        if (InputManager::getInstance().isKeyDown("right", m_playerIndex))
        {
            translation.x += 1;
            SpriteOffset += toSpriteOffset(0, m_playerIndex);
        }
        if (InputManager::getInstance().isKeyDown("left", m_playerIndex))
        {
            translation.x -= 1;
            SpriteOffset += toSpriteOffset(0, m_playerIndex);
        }

        if (InputManager::getInstance().isKeyDown("up", m_playerIndex))
        {
            translation.y -= 1;
            SpriteOffset += toSpriteOffset(2, m_playerIndex);
        }
        if (InputManager::getInstance().isKeyDown("down", m_playerIndex))
        {
            translation.y += 1;
            SpriteOffset += toSpriteOffset(1, m_playerIndex);
        }
        if (m_gameObject.getComponent<RigidBodyComponent>().get()->GetTether() != nullptr)
        {
            m_gameObject.getComponent<RigidBodyComponent>()
                .get()
                ->GetTether()
                ->getGameObject()
                .getComponent<SpriteRenderComponent>()
                .get()
                ->IncrementOffset(SpriteOffset);
        }
    Vector2f moveVec = translation;
        if (moveVec.x != 0 && moveVec.y != 0)
        {
            moveVec = moveVec / sqrt(moveVec.x * moveVec.x + moveVec.y * moveVec.y);
        }

    if (m_gameObject.getComponent<RigidBodyComponent>() != nullptr && rigidControll)
    {
            if (m_gameObject.getComponent<RigidBodyComponent>().get()->GetTether() != nullptr)
            {
                Vector2f Y = m_gameObject.getComponent<RigidBodyComponent>().get()->GetTether()->orientation;
                
                float    angle = std::atan2(Y.y, Y.x);

                Vector2f T = Vector2f(-std::sin(angle), std::cos(angle));

                moveVec = T * moveVec.y + Y * moveVec.x;
               
                //moveVec = T * moveVec.y;
            }
           
        m_gameObject.getComponent<RigidBodyComponent>()->addVelocity(moveVec * speed * deltaTime);
    }
    else
    {
        //note: still needs to be normalized
        m_gameObject.move(translation * speed * deltaTime);
    }


}
} // namespace mmt_gd
