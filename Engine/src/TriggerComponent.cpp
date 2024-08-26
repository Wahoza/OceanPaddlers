//Author: Kilian Kolb
//Description: Is used for colliders that only cause an event on collision and has no solid collider.
//License: LICENSE.txt
#include "stdafx.h"

#include "GameObject.hpp"
#include "TriggerComponent.hpp"
#include "PhysicsManager.hpp"
#include "EventBus.hpp"
#include "GameObjectEvents.hpp"
#include "Hud.hpp"
#include "PlayerComponent.hpp"
#include "AIController.hpp"
#include "RigidBodyComponent.hpp"
#include<cmath>
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
void TriggerComponent::Notify(GameObject& gameObject)
{
    //std::cout << m_notification << std::endl;
    //
    if (!enabled)
    {
        return;
    }
        if (m_notification == "Goal")
        {
            if (PlayerComponent* comp = gameObject.getComponent<PlayerComponent>().get(); comp != nullptr)
            {
                comp->Victory();
                //removeSelf();
            }
            //Hud::getInstance()->won = true;
        }
        if (m_notification == "Victory")
        {
            IEvent::Ptr Victory = std::make_shared<VictorySoundEvent>(true);
            EventBus::getInstance().fireEvent(Victory);
        }
        if (m_notification == "Current")
        {
            IEvent::Ptr Wave = std::make_shared<WaveSoundEvent>(true);
            EventBus::getInstance().fireEvent(Wave);
        }
        if (m_notification == "Transition")
        {
            if (PlayerComponent* comp = gameObject.getComponent<PlayerComponent>().get(); comp != nullptr)
            {
                //comp->Victory();
                bool  eventData(true);
                IEvent::Ptr swapRequestEvent = std::make_shared<SwapTileRequest>(eventData);
                EventBus::getInstance().fireEvent(swapRequestEvent);

                //removeSelf();
            }
            //Hud::getInstance()->won = true;
        }
        if (m_notification == "Attack")
        {

            if (m_origin != gameObject.getId())
            {
                if (AIController* enemy = gameObject.getComponent<AIController>().get(); enemy != nullptr)
                {
                    enemy->TakeDamage();

                }
            }
        }
    
}
Vector2f TriggerComponent::getPosition()
{
    return {m_shape.left, m_shape.top};
}
void TriggerComponent::setEnabled(bool _enabled)
{
    enabled = _enabled;
}

void TriggerComponent::removeSelf()
{
    //note: this breaks game if used improperly
    return PhysicsManager::getInstance()->UnregisterTrigger(this);
}
}