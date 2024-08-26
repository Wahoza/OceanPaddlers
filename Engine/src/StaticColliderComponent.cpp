//Author: Kilian Kolb
//Description: This is for Colliders that dont move and are solid
//License: LICENSE.txt
#include "stdafx.h"

#include "GameObject.hpp"
#include "PhysicsManager.hpp"
#include "StaticColliderComponent.hpp"
namespace mmt_gd
{
StaticColliderComponent::StaticColliderComponent(FloatRect shape, string type) :
m_shape(shape)
{
    PhysicsManager::getInstance()->RegisterCollider(this);
    m_coll = ConvertToCollType(type);
}

Vector2f StaticColliderComponent::getPosition()
{
    return m_shape.getPosition();
}
void StaticColliderComponent::ChangePosition(Vector2f changePos)
{

}
void StaticColliderComponent::removeSelf()
{
    PhysicsManager::getInstance()->UnregisterStaticCollider(this);
}
void StaticColliderComponent::ChangeType(string type)
{
    m_coll = ConvertToCollType(type);
}
bool StaticColliderComponent::init()
{
    return true;
}
void StaticColliderComponent::update(float deltaTime)
{

}
} // namespace mmt_gd