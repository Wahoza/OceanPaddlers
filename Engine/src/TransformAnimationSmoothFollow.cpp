//Author:Radomir Dinic, modified by Kilian Kolb
//Description: Is used for an Object to smoothly follow another along the x axis, in this use case it is the Camera that follows the player.
//License: LICENSE.txt
#include "stdafx.h"

#include "TransformAnimationSmoothFollow.hpp"

#include "GameObject.hpp"

#include <utility>


mmt::TransformAnimationSmoothFollow::TransformAnimationSmoothFollow(mmt_gd::GameObject::Ptr target, const float smoothFactor) :
m_target(std::move(target)),
m_smoothFactor(smoothFactor)
{
}

void mmt::TransformAnimationSmoothFollow::update(float deltaTime, sf::Transformable& transform)
{
    if (m_target && m_target->isMarkedForDelete())
    {
        m_target = nullptr;
    }

    if (!m_target)
    {
        return;
    }

    sf::Vector2f toTarget = furthestTartgetPos - transform.getPosition();

    transform.move(toTarget * m_smoothFactor * deltaTime);
    transform.setPosition(transform.getPosition().x, middleYval);
    if (m_target->getPosition().x > furthestTartgetPos.x)
    {
        furthestTartgetPos = m_target->getPosition();
    }
}
