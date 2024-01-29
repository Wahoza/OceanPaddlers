#include "stdafx.h"

#include "GameObject.hpp"
#include "TriggerComponent.hpp"
#include "PhysicsManager.hpp"
#include "EventBus.hpp"
#include "GameObjectEvents.hpp"
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
    std::cout << m_notification << std::endl;
    std::pair<int, std::string> eventData(1, m_notification);
    IEvent::Ptr                 scoreEvent = std::make_shared<ScoreEvent>(eventData);
    EventBus::getInstance().fireEvent(scoreEvent);

}
Vector2f TriggerComponent::getPosition()
{
    return {m_shape.left, m_shape.top};
}
}