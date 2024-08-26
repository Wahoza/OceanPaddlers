//Author: Kilian Kolb
//Description: Handles Player status and notifies objects with Events
//License: LICENSE.txt
#include "stdafx.h"
#include "PlayerComponent.hpp"
#include "RigidBodyComponent.hpp"

#include "GameObjectEvents.hpp"
#include "EventBus.hpp"
#include "SpriteRenderComponent.hpp"
#include "AiManager.hpp"

namespace mmt_gd
{
PlayerComponent::PlayerComponent(GameObject& gameObject) : IComponent(gameObject)
{
    AiManager::getInstance()->setGoalObject(&gameObject);
    EventBus::getInstance().addListener(SwapTiles::Type, [this](IEvent::Ptr event) { onEnvironmentSwap(event); });
}
void PlayerComponent::CollectibleCollision(string _item)
{
    if (_item == "Egg")
    {
        EggScore++;
        int         eventData(EggScore);
        IEvent::Ptr eggEvent = std::make_shared<EggEvent>(eventData);
        EventBus::getInstance().fireEvent(eggEvent);
    }
    if (_item == "Health")
    {
        //change this to a new unique event
        currentlives++;
        int eventData(currentlives);
        IEvent::Ptr healthGet = std::make_shared<HealthCollect>(eventData);
        EventBus::getInstance().fireEvent(healthGet);
    }
}
void PlayerComponent::DamageCollision()
{
    if (!(hurtTime < maxHurtTime) && timeSinceLastSwap > swapDuration)
    {
        hurtTime = 0;
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
    float       distanceTraveled = m_gameObject.getPosition().x / distancePixeltoM;
    totalScore             = distanceTraveled * 3 + EggScore * 12;
    std::tuple       eventData(distanceTraveled, EggScore, won, totalScore);
    IEvent::Ptr GameOver = std::make_shared<GameOverEvent>(eventData);
    EventBus::getInstance().fireEvent(GameOver);
    EggScore = 0;
}
void PlayerComponent::Victory()
{
    won = true;
    GameEnd();
}
void mmt_gd::PlayerComponent::update(float deltaTime)
{
    if (hurtTime < maxHurtTime)
    {
        hurtTime += deltaTime;
        if (((int)(hurtTime * 3)) % 2 == 0 && hurtTime < maxHurtTime - 0.3)
        {
            m_gameObject.getComponent<SpriteRenderComponent>().get()->setSpriteOpacity(100);
        }
        else
        {
            m_gameObject.getComponent<SpriteRenderComponent>().get()->setSpriteOpacity(255);
        }
    }

    if (timeSinceLastSwap < swapDuration && timeSinceLastSwap > 1)
    {
        m_gameObject.setPosition({m_gameObject.getPosition().x, ObjectYonSwap});

        for (auto& rb : m_gameObject.getComponents<RigidBodyComponent>())
        {
            rb.get()->setVelocity({0, 0});
            rb.get()->GetTether()->setVelocity({0, 0});
        }
    }

    if (auto pos = m_gameObject.getPosition().y; !(pos > boundings[0] && pos < boundings[1])) {
        DamageCollision();
    }
    timeSinceLastSwap += deltaTime;
}
void PlayerComponent::onEnvironmentSwap(IEvent::Ptr event)
{
    auto swapEvent    = std::dynamic_pointer_cast<SwapTiles>(event);

    swapDuration      = std::get<1>(swapEvent.get()->getData());
    timeSinceLastSwap = 0;
}
float PlayerComponent::getHurtTime()
{
    return hurtTime;
}
float PlayerComponent::getMaxHurtTime()
{
    return maxHurtTime;
}
bool PlayerComponent::init()
{
    return true;
}
} // namespace mmt_gd