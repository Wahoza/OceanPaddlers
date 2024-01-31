#include "stdafx.h"

#include "GameObject.hpp"
#include "TriggerComponent.hpp"
#include "PhysicsManager.hpp"
#include "EventBus.hpp"
#include "GameObjectEvents.hpp"
#include "Hud.hpp"
namespace mmt_gd
{
TriggerComponent::TriggerComponent(FloatRect shape, string identifier, string notification) :
m_shape(shape),
m_identifier(identifier),
m_notification(notification)
{
    PhysicsManager::getInstance()->RegisterTrigger(this);
}
bool TriggerComponent::init()
{
    return true;
}
void TriggerComponent::Notify()
{
    //std::cout << m_notification << std::endl;
    if (m_notification == "Goal")
    {
        Hud::getInstance()->won = true;
        Hud::getInstance()->PingGameEnd();
    }
}
Vector2f TriggerComponent::getPosition()
{
    return {m_shape.left, m_shape.top};
}
}