//Author: Kilian Kolb
//Description: Gives gameobjects Collectible type and allows them to trigger unique interactions upon collision.
//License: LICENSE.txt

#include "stdafx.h"
#include "IComponent.hpp"
#include "CollectibleComponent.hpp"
#include "GameObject.hpp";
#include "PhysicsManager.hpp"
#include "EventBus.hpp"
#include "GameObjectEvents.hpp"
#include "SoundManager.hpp"
#include "PlayerComponent.hpp"
using namespace sf;
using namespace std;
namespace mmt_gd
{
CollectibleComponent::CollectibleComponent(GameObject& gameObject, string _type, float width, float height) :
IComponent(gameObject),
type(_type)
{
    m_shape.width = width;
    m_shape.height = height;
    PhysicsManager::getInstance()->RegisterCollectible(this);
}
void CollectibleComponent::update(float deltaTime)
{
    m_shape.left = m_gameObject.getPosition().x;
    m_shape.top  = m_gameObject.getPosition().y;
}
bool CollectibleComponent::init()
{
    return true;
}

void CollectibleComponent::CollissionMessage(GameObject& CollObject)
{
	//send event later
    //std::cout << type << std::endl;
    if (CollObject.getComponent<PlayerComponent>() != nullptr)
    {
        CollObject.getComponent<PlayerComponent>().get()->CollectibleCollision(type);
        m_gameObject.setActive(false);
    }
}
}