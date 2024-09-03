

#include "stdafx.h"
#include "PlayerAttackComponent.hpp"

#include "IComponent.hpp"
#include "GameObject.hpp"

#include "RigidBodyComponent.hpp"
#include "TetherComponent.hpp"
#include "TriggerComponent.hpp"

#include "InputManager.hpp"
#include "EventBus.hpp"
#include "GameObjectEvents.hpp"
#include "SpriteRenderComponent.hpp"
#include "PlayerComponent.hpp"
#include "IEvent.hpp"
namespace mmt_gd
{
PlayerAttackComponent::PlayerAttackComponent(
    GameObject&  gameObject,
    sf::Vector2f size,
    float        expansion,
    float        HitboxScale,
    int PlayerID) :
IComponent(gameObject)
{
    //assumes halfwaypoint is center of a rect, center is game Object. the offset is stored in the second half of the tuple to the gameObject.
    //expansion is the width/height depending on what side the connetion is.
    size *= HitboxScale;
    Vector2f halfPoint = size / 2.f;
    for (int i = 0; i <= 3; i++)
    {        
        Vector2       influence = {i == 0 ? -1 : 1, (i == 3 ? -1 : 1)};
        sf::FloatRect shape = {0, 0, 0, 0};
        sf::Vector2f offset;

        if (i % 2 == 1)
        {
            //vertical
            shape.height = expansion * influence.y;
            shape.width = size.x + 2 * expansion;
            offset       = Vector2f{-halfPoint.x, halfPoint.y * influence.y} + halfPoint -
                     Vector2f{expansion, 0}; //Vector2f{halfPoint.x, 0};
            
        }
        else
        {
            shape.height = size.y + 2 * expansion;
            shape.width  = expansion * influence.x;

            offset = Vector2f{influence.x * halfPoint.x, -halfPoint.y} + halfPoint -
                     Vector2f{0, expansion}; //Vector2f{halfPoint.x, 0};
        }

        TriggerComponent* trigger = new TriggerComponent(shape, "Enemy", "Attack");
        trigger->setEnabled(false);
        if (RigidBodyComponent* rb = gameObject.getComponent<RigidBodyComponent>().get(); rb != nullptr)
        {
            if (rb->GetTether() != nullptr)
            {
                trigger->m_origin = rb->GetTether()->getGameObject().getId();
            }
        }
        else
        {
            trigger->m_origin = gameObject.getId();
        }

        triggers.push_back(std::tuple<TriggerComponent*, Vector2f>(trigger, offset));

        pID = PlayerID;
    }
    EventBus::getInstance().addListener(PowerUP::Type, [this](IEvent::Ptr event) { ListenPowerupState(event); });
}
void PlayerAttackComponent::ListenPowerupState(IEvent::Ptr event)
{
    if (auto pwrEvent = std::dynamic_pointer_cast<PowerUP>(event))
    {
        poweredUp = pwrEvent->getData();
    }
}
bool PlayerAttackComponent::init()
{
    return true;
}
void PlayerAttackComponent::update(const float deltaTime)
{
    RealignTriggers(triggers);
    if (attackTimer > 0)
    {
        attackTimer -= deltaTime;
        if (attacking)
        {
            DoAttackFrame(true);
        }
        if (attackTimer < attackCooldown - attackTimer && attacking)
        {
            for (auto& trigger : triggers)
            {
                std::get<0>(trigger)->setEnabled(false);
            }
            attacking = false;
        }
    }
    

    auto& inputManager = InputManager::getInstance();
    //create vector if keyboard Input is used:
    Vector2f inputVec = Vector2f
    {
        inputManager.isKeyDown("left", pID) ? -1.f : 0 + inputManager.isKeyDown("right", pID) ? 1.f : 0,
        inputManager.isKeyDown("down", pID) ? -1.f : 0 + inputManager.isKeyDown("up", pID) ? 1.f : 0
    };

    if (sf::Joystick::isConnected(pID)){
    
        float xAxis = sf::Joystick::getAxisPosition(pID, sf::Joystick::X);
        float yAxis = sf::Joystick::getAxisPosition(pID, sf::Joystick::Y);

        inputVec = {xAxis, yAxis};

    }
    //std::cout << getGameObject().getComponent<RigidBodyComponent>().get()->GetTether()->getCurrentAngle() << std::endl;
    if (poweredUp)
    {
        for (auto& trigger : triggers)
        {
            std::get<0>(trigger)->setEnabled(true);
        }
        attackTimer = 0.1;
        attacking   = true;
    }
    if (((inputManager.isKeyPressed("attack", pID) || sf::Joystick::isButtonPressed(pID, 0)) && attackTimer < 0 && !attacking))
    {
        Vector2f moveVec = RotateVec(inputVec,getGameObject().getComponent<RigidBodyComponent>().get()->GetTether()->getCurrentAngle() * -1);
        bool attacked = false;
        if (moveVec.y > directionThreshold)
        {
            std::get<0>(triggers[3])->setEnabled(true);
            attacked = true;
        }
        if (moveVec.x < -directionThreshold && pID == 0) //as P2 cant attack Backwards
        {
            std::get<0>(triggers[0])->setEnabled(true);
            attacked = true;
        }
        if (moveVec.y < -directionThreshold)
        {
            std::get<0>(triggers[1])->setEnabled(true);
            attacked = true;
        }
        if (moveVec.x > directionThreshold && pID == 1) //as P1 cant attack Forwards
        {
            std::get<0>(triggers[2])->setEnabled(true);
            attacked = true;
        }

        if (attacked)
        {
            attackTimer = attackCooldown;
            bool         eventData(true);
            IEvent::Ptr attackEvent = std::make_shared<AttackEvent>(eventData);
            EventBus::getInstance().fireEvent(attackEvent);

            DoAttackFrame(true);
            
            attacking = true;
        }
    }
}

void PlayerAttackComponent::RealignTriggers(std::vector<std::tuple<TriggerComponent*, Vector2f>> _triggers)
{
    for (std::tuple<TriggerComponent*, Vector2f>& trigger : _triggers)
    {
        std::get<0>(trigger)->m_shape.left = getGameObject().getPosition().x + std::get<1>(trigger).x;
        std::get<0>(trigger)->m_shape.top = getGameObject().getPosition().y + std::get<1>(trigger).y;
    }
}

Vector2f PlayerAttackComponent::RotateVec(Vector2f input, float angle)
{
    
        // Convert angle from degrees to radians
        float angleRadians = angle * 3.14f / 180.0f;

        // Compute the rotation matrix components
        float cosAngle = std::cos(angleRadians);
        float sinAngle = std::sin(angleRadians);

        // Perform the rotation
        Vector2f rotatedVec;
        rotatedVec.x = input.x * cosAngle - input.y * sinAngle;
        rotatedVec.y = input.x * sinAngle + input.y * cosAngle;

        return rotatedVec;
    
}
void PlayerAttackComponent::DoAttackFrame(bool active)
{
    if (RigidBodyComponent* rb = getGameObject().getComponent<RigidBodyComponent>().get(); rb != nullptr)
    {
        if (rb->GetTether() != nullptr)
        {
            if (SpriteRenderComponent* sprite = rb->GetTether()->getGameObject().getComponent<SpriteRenderComponent>().get();
                sprite != nullptr)
            {

                sprite->IncrementOffset(Vector2i{0, (pID + 1) * 4});//forgor how sprite works lol, just always put true
            }
        }
    }
}
} // namespace mmt_gd
