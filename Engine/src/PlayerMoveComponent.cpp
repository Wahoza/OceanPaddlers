//Author:Radomir Dinic, modified by Kilian Kolb
//Description: Changes Position of a Game Object based on Inputs. Also supports Controller circle pad inputs.
//License: LICENSE.txt
#include "stdafx.h"

#include "PlayerMoveComponent.hpp"

#include "GameObject.hpp"
#include "InputManager.hpp"
#include "RigidBodyComponent.hpp"
#include "SpriteRenderComponent.hpp"
#include <cmath>
using namespace sf;
using namespace std;
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
        return (Vector2i{0, value});
    }
    if (playerIdx == 1)
    {
        return (Vector2i{value, 0});
    }
}

void PlayerMoveComponent::calculateSpriteOffset(Vector2f input, int index)
{

    Vector2i SpriteOffset = Vector2i{0, 0};


    if (abs(input.x) - abs(input.y)*0.3 > abs(input.y))
    {
        //
        //  << abs(input.x) <<std::endl;
        SpriteOffset = toSpriteOffset(3, index);
    }
    else
    {
        if ((input.y) > 0)
        {
            SpriteOffset = toSpriteOffset(1, index);
        }
        else if ((input.y) < 0)
        {
            SpriteOffset = toSpriteOffset(2, index);
        }

    }
   

    m_gameObject.getComponent<RigidBodyComponent>()
        .get()
        ->GetTether()
        ->getGameObject()
        .getComponent<SpriteRenderComponent>()
        .get()
        ->IncrementOffset(SpriteOffset);

}

void PlayerMoveComponent::update(const float deltaTime)
{
    const auto   speed = 1.0F; // pixels/second
    float      additionalYweight = 3;

    sf::Vector2f translation{};
    float        xAxis        = sf::Joystick::getAxisPosition(m_playerIndex, sf::Joystick::X);
    float        yAxis        = sf::Joystick::getAxisPosition(m_playerIndex, sf::Joystick::Y);

    // Threshold to avoid small accidental movements
    if (std::abs(xAxis) > 20.0f)
    {
        translation.x += speed * xAxis * deltaTime / 100.0f;
    }


    if (std::abs(yAxis) > 20.0f)
    {
        translation.y += speed * yAxis * deltaTime / 100.0f * additionalYweight;

        //note: this is required for Controller, Reason: Perfectly holding up doesnt impact the movement of player enough
        translation.x += 0.1 * deltaTime / 100.0f;
    }



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
        
        calculateSpriteOffset(translation, m_playerIndex);
        

        Vector2f moveVec = translation;

        if (moveVec.x != 0 && moveVec.y != 0)
        {
            moveVec = moveVec / sqrt(moveVec.x * moveVec.x + moveVec.y * moveVec.y);
        }
        

    if (m_gameObject.getComponent<RigidBodyComponent>() != nullptr && rigidControll)
    {
            if (m_gameObject.getComponent<RigidBodyComponent>().get()->GetTether() != nullptr)
            {
                //Note: Slowing-> holding backwards was considered OP during playtesting, so:
            if (moveVec.x < 0)
            {
                moveVec.x = moveVec.x / 3;
            }

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
