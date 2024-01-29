#include "stdafx.h"
#include "IComponent.hpp"
#include "CollectibleComponent.hpp"
#include "GameObject.hpp";
#include "PhysicsManager.hpp"

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
Vector2f CollectibleComponent::getPosition()
{
    if (!m_gameObject.isMarkedForDelete())
    {   
        return m_gameObject.getPosition();
    }
    else
    {
        return {0, 0};
    }
}
void CollectibleComponent::CollissionMessage()
{
	//send event later
    std::cout << type << std::endl;
    m_gameObject.markForDelete();
    //delete &m_gameObject;
}
}