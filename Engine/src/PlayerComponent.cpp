//Author: Kilian Kolb
//Description: Handles Player status and notifies objects with Events
//License: LICENSE.txt
#include "stdafx.h"
#include "PlayerComponent.hpp"
#include "GameObjectEvents.hpp"
#include "EventBus.hpp"
#include "SpriteRenderComponent.hpp"
#include "AiManager.hpp"
namespace mmt_gd
{
PlayerComponent::PlayerComponent(GameObject& gameObject) : IComponent(gameObject)
{
    AiManager::getInstance()->setGoalObject(&gameObject);
}
void PlayerComponent::CollectibleCollision()
{
    EggScore++;
    int         eventData(EggScore);
    IEvent::Ptr eggEvent = std::make_shared<EggEvent>(eventData);
    EventBus::getInstance().fireEvent(eggEvent);
}
void PlayerComponent::DamageCollision()
{
    if (!(hurtTime > 0))
    {
        hurtTime = 5;
        currentlives--;

        int         eventData(currentlives);
        IEvent::Ptr Hurt = std::make_shared<HurtEvent>(eventData);
        EventBus::getInstance().fireEvent(Hurt);

        if (currentlives == 0)
        {
            GameEnd();
        }
    }
}
void PlayerComponent::GameEnd()
{
    float       distanceTraveled = m_gameObject.getPosition().y / distancePixeltoM;
    std::tuple       eventData(distanceTraveled, EggScore, won);
    IEvent::Ptr GameOver = std::make_shared<GameOverEvent>(eventData);
    EventBus::getInstance().fireEvent(GameOver);
}
void PlayerComponent::Victory()
{
    won = true;
    GameEnd();
}
void mmt_gd::PlayerComponent::update(float deltaTime)
{
    if (hurtTime > 0)
    {
        hurtTime -= deltaTime;
        if (((int)(hurtTime * 3)) % 2 == 0 && hurtTime > 0.3)
        {
            m_gameObject.getComponent<SpriteRenderComponent>().get()->setSpriteOpacity(100);
        }
        else
        {
            m_gameObject.getComponent<SpriteRenderComponent>().get()->setSpriteOpacity(255);
        }
    }
}
float PlayerComponent::getHurtTime()
{
    return hurtTime;
}
bool PlayerComponent::init()
{
    return true;
}
} // namespace mmt_gd