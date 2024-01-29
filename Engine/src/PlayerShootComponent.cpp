#include "stdafx.h"

#include "PlayerShootComponent.hpp"

#include "InputManager.hpp"

#include <SFML/System.hpp>
#include <iostream>


//static void onBulletCollision(const mmt_gd::BoxColliderComponent& bullet, const mmt_gd::BoxColliderComponent& other)
//{
//    bullet.getGameObject().setActive(false);
//
//    //std::cout << bullet.getGameObject().getId()<< " \tcollision -> \t"<< other.getGameObject().getId()  << std::endl;
//}

mmt_gd::PlayerShootComponent::PlayerShootComponent(
    GameObject&         gameObject,
    const size_t        poolSize,
    sf::RenderWindow&   renderWindow,
    const std::string&  textureFile,
    const sf::IntRect   textureRect,
    const sf::FloatRect colliderRect,
    const float         bulletMass,
    const std::string&  layerName) :
IComponent(gameObject)
{
}

void mmt_gd::PlayerShootComponent::shoot(sf::Vector2f directionSpeed)
{
    throw std::logic_error("The method or operation is not implemented.");
}

bool mmt_gd::PlayerShootComponent::init()
{
    return true;
}

void mmt_gd::PlayerShootComponent::update(float deltaTime)
{
    m_timeSinceLastShot += deltaTime;
    if (InputManager::getInstance().isKeyDown("fire", 0))
    {
        //shoot(sf::Vector2f{1000, 0});
        std::cout << "shoot" << std::endl;    
    }
}
