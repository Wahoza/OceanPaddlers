#include "TetherComponent.hpp"

#include "stdafx.h"
#include "IComponent.hpp"
#include "RigidBodyComponent.hpp"

#include "GameObjectManager.hpp"
#include "PlayerMoveComponent.hpp"
#include "SpriteRenderComponent.hpp"
#include <cmath>
using namespace sf;
using namespace std;

namespace mmt_gd
{
TetherComponent::TetherComponent(
    GameObject& gameObject,
    FloatRect   shape,
    Vector2f    offset,
    string      forces,
    float       mass,
    float       loss,
    bool        controll,
    float _movePercentage) :
IComponent(gameObject),
movePercentage(_movePercentage),
length(shape.width / 2),
TetherItem1(createNewTetheredObject("tethered1")),
TetherItem2(createNewTetheredObject("tethered2"))
{
    
    length = shape.width / 2;
    float height = shape.height;

    TetherItem1.addComponent<RigidBodyComponent>(TetherItem1,
                            FloatRect{0, 0, length, height},
                            Vector2f{0.f, 0.f},
                            forces,
                            mass,
                            loss);
    TetherItem1.getComponent<RigidBodyComponent>()->SetTether(this);


    TetherItem2.addComponent<RigidBodyComponent>(TetherItem2,
                            FloatRect{0, 0, length, height},
                            Vector2f{0.f, 0.f},
                            forces,
                            mass,
                            loss);
    TetherItem2.getComponent<RigidBodyComponent>()->SetTether(this);

    if (controll)
    {
        TetherItem1.addComponent<PlayerMoveComponent>(TetherItem1, 0, true);
        TetherItem2.addComponent<PlayerMoveComponent>(TetherItem2, 1, true);
    }

    //reposition for editor's sake
    Vector2f scale = {m_gameObject.getScale().x / 2, m_gameObject.getScale().y / 2};
    m_gameObject.setPosition(m_gameObject.getPosition() + scale);

    Vector2f pos1 = (- orientation / 2.f + m_gameObject.getPosition()) * length;
    TetherItem1.setPosition(pos1);

    Vector2f pos2 = (orientation / 2.f + m_gameObject.getPosition()) * length;
    TetherItem2.setPosition(pos2);
    //m_gameObject.setOrigin(shape.width / 2, shape.height / 2);
}
void TetherComponent::update(const float deltaTime)
{
    TetherItem1.update(deltaTime);
    TetherItem2.update(deltaTime);

    GetForce(deltaTime);
    CalculateEffect();
    RepositionBodies();
    //note: rotate sprite later
    if (m_gameObject.getComponent<SpriteRenderComponent>() != nullptr)
    {
        auto& sprite = m_gameObject.getComponent<SpriteRenderComponent>().get()->getSprite();
        float angle  = atan2(orientation.y, orientation.x);
        angle        = angle * 180.0f / static_cast<float>(3.14);
        Vector2f spriteOffset = {sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2};
        sprite.setOrigin(spriteOffset);
        sprite.setRotation(angle);
    }
}
void TetherComponent::GetForce(float deltaTime)
{
    forces.push_back(TetherItem1.getComponent<RigidBodyComponent>()->getVelocity() * deltaTime);
    forces.push_back(TetherItem2.getComponent<RigidBodyComponent>()->getVelocity() * deltaTime);
}
void TetherComponent::CalculateEffect()
{
    for (Vector2f force : forces)
    {

        m_gameObject.setPosition(m_gameObject.getPosition() + force * movePercentage);
        force = force - force*movePercentage;
    }
    
    //only two objects are tethered
    //behind:
    orientation = normalize(orientation / 2.f - forces[0]);

    orientation = normalize(orientation / 2.f + forces[1]);
    forces.clear();
}
void TetherComponent::RepositionBodies()
{
    Vector2f size1 = {TetherItem1.getComponent<RigidBodyComponent>().get()->m_shape.width,
                      TetherItem1.getComponent<RigidBodyComponent>().get()->m_shape.height};
    Vector2f size2 = {TetherItem2.getComponent<RigidBodyComponent>().get()->m_shape.width,
                      TetherItem2.getComponent<RigidBodyComponent>().get()->m_shape.height};
    TetherItem1.setPosition((-orientation / 2.f * length + m_gameObject.getPosition() - size1/ 2.f));
    TetherItem2.setPosition((orientation / 2.f * length + m_gameObject.getPosition() - size2 / 2.f));
}
Vector2f TetherComponent::normalize(Vector2f vec)
{
    return vec / sqrt(vec.x * vec.x + vec.y * vec.y);
}
bool TetherComponent::init()
{
    return true;
}
GameObject& TetherComponent::createNewTetheredObject(string id)
{
    string     init = m_gameObject.getId() + id;
    //string     init = id;
    return *(new GameObject(init));

}
TetherComponent::~TetherComponent()
{
    //TetherItem1.removeComponent(TetherItem1.getComponent<RigidBodyComponent>());
    //TetherItem1.removeComponent(TetherItem1.getComponent<PlayerMoveComponent>());
    //TetherItem2.removeComponent(TetherItem2.getComponent<RigidBodyComponent>());
    //TetherItem2.removeComponent(TetherItem2.getComponent<PlayerMoveComponent>());

    delete &TetherItem1;
    delete &TetherItem2;
}
//Still needs special deconstructer
}