//Author: Kilian Kolb
//Description: RigidBody Component for Physics.
//License: LICENSE.txt
#include "stdafx.h"
#include "RigidBodyComponent.hpp"
#include "GameObject.hpp"
#include "PhysicsManager.hpp"
#include "TetherComponent.hpp"

namespace mmt_gd
{
RigidBodyComponent::RigidBodyComponent(
    GameObject& gameObject, FloatRect shape, Vector2f offset, string forces, float mass, float loss) :
IComponent(gameObject),
m_shape(shape),
m_gameObjectOffset(offset),
m_mass(mass),
m_invMass(1.0f / mass),
m_forces(StringToVecList(forces)),
Loss(loss)
{
    m_shape.top = gameObject.getPosition().y;
    m_shape.left = gameObject.getPosition().x;
    PhysicsManager::getInstance()->RegisterBody(this);

}

const float RigidBodyComponent::getInvMass()
{
    return m_invMass;
}

Vector2f RigidBodyComponent::getVelocity()
{
    return m_velocity;
}
void RigidBodyComponent::setVelocity(Vector2f velocity)
{
    m_velocity = velocity;
}
void RigidBodyComponent::addVelocity(Vector2f velocity)
{
    m_velocity += velocity;

    if (m_velocity.x * m_velocity.x + m_velocity.y * m_velocity.y >= trueMaxVelocity * trueMaxVelocity)
    {  
        if (m_velocity.x * m_velocity.x + m_velocity.y * m_velocity.y > 0)
        {
           
            m_velocity = m_velocity/sqrt(m_velocity.x * m_velocity.x + m_velocity.y * m_velocity.y) * trueMaxVelocity;

        }
    }
}
Vector2f RigidBodyComponent::getPosition()
{
    return m_gameObject.getPosition();
}
void RigidBodyComponent::ChangePosition(Vector2f changePos)
{
    m_gameObject.move(changePos);
}
bool RigidBodyComponent::init()
{
    return true;
}
void RigidBodyComponent::update(float deltaTime)
{
    calculateOffset();
    for (auto& force : m_forces)
    {
        addVelocity(force * deltaTime);
    }

    m_gameObject.move(m_velocity * deltaTime);

    m_velocity -= m_velocity * deltaTime * Loss;
    
}
void RigidBodyComponent::calculateOffset()
{
    m_shape.left = m_gameObject.getPosition().x + m_gameObjectOffset.x;
    m_shape.top  = m_gameObject.getPosition().y + m_gameObjectOffset.y;
}
void RigidBodyComponent::SetTether(TetherComponent* Tether)
{
    m_tether = Tether;
}
TetherComponent* RigidBodyComponent::GetTether()
{
    return m_tether;
}
RigidBodyComponent::~RigidBodyComponent()
{
    PhysicsManager::getInstance()->UnregisterBody(this);
}

} // namespace mmt_gd