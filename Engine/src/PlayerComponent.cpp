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
#include "CollisionParticleHandler.hpp"

#include <ctime>
#include <cstdlib>

namespace mmt_gd
{
PlayerComponent::PlayerComponent(GameObject& gameObject) : IComponent(gameObject)
{
    AiManager::getInstance()->setGoalObject(&gameObject);
    EventBus::getInstance().addListener(SwapTiles::Type, [this](IEvent::Ptr event) { onEnvironmentSwap(event); });

    if (!alternateTexture.loadFromFile("../assets/turtles_monochrome.png"))
    {
        sf::err() << "Could not load monochrome texture from playr" << std::endl;
    }
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
    if (_item == "PowerUp")
    {
        PowerUpTimer = PowerUpDuration;
        bool         eventData(true);
        IEvent::Ptr PWRUP = std::make_shared<PowerUP>(eventData);
        EventBus::getInstance().fireEvent(PWRUP);
    }
}
void PlayerComponent::DamageCollision()
{
    if (!(hurtTime < maxHurtTime) && timeSinceLastSwap > swapDuration && PowerUpTimer <= 0)
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

    if (0 < PowerUpTimer)
    {
        PowerUpTimer -= deltaTime;
        ParticleTriggerTimer += deltaTime;
        if (!ColorStored)
        {

            if (spriteColor == sf::Color{0, 0, 0, 0})
            {
                spriteColor = m_gameObject.getComponent<SpriteRenderComponent>()->getSprite().getColor();
                spriteTexture = *m_gameObject.getComponent<SpriteRenderComponent>()->getSprite().getTexture();
                ColorStored = true;
            }
        }
        if (firstPowerUpUpdate && !PowerUpTimer < 0.5)
        {
            firstPowerUpUpdate = false;
            m_gameObject.getComponent<SpriteRenderComponent>()->getSprite().setTexture(alternateTexture);

        }
        if (ParticleTriggerInterveal < ParticleTriggerTimer && PowerUpTimer > 0.5)
        {
            sf::Color baseColor  = getRandomBrightColor();
            ParticleTriggerTimer = 0.1 - float(std::rand()) / (RAND_MAX * 2);
            ParticleManager::getInstance()
                ->generateParticlesAroundPoint(m_gameObject.getPosition(),
                                               20,
                                               20,
                                               20,
                                               {2, 4}, baseColor,
                                               1,
                                               getRandomBrightColor(),
                                               5,0);
            m_gameObject.getComponent<SpriteRenderComponent>()->getSprite().setColor(baseColor);
        }
        if (PowerUpTimer < 0.5)
        {
            if (!firstPowerUpUpdate)
            {
                bool        eventData(false);
                IEvent::Ptr PWRUP = std::make_shared<PowerUP>(eventData);
                EventBus::getInstance().fireEvent(PWRUP);

                firstPowerUpUpdate = true;
            }
            m_gameObject.getComponent<SpriteRenderComponent>()->getSprite().setTexture(spriteTexture);
            m_gameObject.getComponent<SpriteRenderComponent>()->getSprite().setColor(spriteColor);
        }
    }
    timeSinceLastSwap += deltaTime;
}
sf::Color PlayerComponent::convertToWhiteOrBlack(const sf::Color& color)
{
    // Calculate luminance
    float luminance = 0.299f * color.r + 0.587f * color.g + 0.114f * color.b;

    // Define a threshold (you can adjust this as needed)
    const float threshold = 250.0f;

    // Convert to white or black based on the threshold
    if (luminance > threshold)
    {
        return sf::Color::White; // Bright enough, convert to white
    }
    else
    {
        return sf::Color::Black; // Dark, convert to black
    }
}
sf::Color PlayerComponent::getRandomBrightColor()
{
    // Define a list of bright colors
    std::vector<sf::Color> brightColors = {
        sf::Color::Red,
        sf::Color::Green,
        sf::Color::Blue,
        sf::Color::Yellow,
        sf::Color::Magenta,
        sf::Color::Cyan,
        sf::Color(255, 165, 0),  // Orange
        sf::Color(255, 105, 180) // Hot Pink
    };

    // Randomly select a color from the list
    int randomIndex = std::rand() % brightColors.size();
    return brightColors[randomIndex];
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
bool mmt_gd::PlayerComponent::getPowerUpState()
{
    return PowerUpTimer > 0;
}
bool PlayerComponent::init()
{
    return true;
}
} // namespace mmt_gd