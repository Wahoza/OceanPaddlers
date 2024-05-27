//Author: Kilian Kolb
//Description: AI behaviour of the Fish enemy
//License: LICENSE.txt


#include "stdafx.h"
#include "AiManager.hpp"
#include "AIController.hpp"
#include "GameObject.hpp"
#include "RigidBodyComponent.hpp"
#include "SpriteRenderComponent.hpp"
#include "TetherComponent.hpp"
#include "PhysicsManager.hpp"
#include "GameObjectEvents.hpp"

namespace mmt_gd
{
void AIController::update(const float deltaTime)
{
    if (m_goal == nullptr)
    {
        return;
    }
    RequestNextState();
    elapsedTimeInPhase += deltaTime;

    switch (currentState)
    {
        case idle:
            //std::cout << "idle" << std::endl;

            idleUpdate(deltaTime);
            break;
        case interested:
            //std::cout << "def" << std::endl;

            interestedUpdate(deltaTime);
            break;
        case agressive:
            //std::cout << "agr" << std::endl;

            agressiveUpdate(deltaTime);
            break;
        case disolve:
            if (m_gameObject.getComponent<RigidBodyComponent>() != nullptr)
            {
                m_gameObject.getComponent<RigidBodyComponent>().get()->isActive = false;
            }
            disolveUpdate(deltaTime);
            break;
    }
}
bool AIController::init()
{
    return true;
}
void AIController::disolveUpdate(float deltaTime)
{
    //disolve on frame 6, 5 frame animation
    int animationFrame = (int)(elapsedTimeInPhase * 5) % 7;
    m_gameObject.getComponent<SpriteRenderComponent>()->IncrementOffset({animationFrame, 1});

    if (animationFrame == 6)
    {
        m_gameObject.setActive(false);
    }
}
void AIController::agressiveUpdate(float deltaTime)
{
    //error here, but where??
    Vector2f moveVec{};

    moveVec = m_goal->getPosition() - m_gameObject.getPosition() - m_gameObject.getComponent<RigidBodyComponent>().get()->m_velocity;

    //Vector2f moveVec = {0, 1};

    moveObj(moveVec, deltaTime);
    m_gameObject.getComponent<SpriteRenderComponent>()->IncrementOffset({((int)(elapsedTimeInPhase * 3) % 4 + 1), 0});
    CompleteSpriteChanges(m_goal->getPosition() - m_gameObject.getPosition());
}

void AIController::interestedUpdate(float deltaTime)
{
    Vector2f moveVec{};
    moveVec.x = m_goal->getPosition().x - m_gameObject.getPosition().x;
    moveVec.y = 0;


    moveObj(moveVec, deltaTime);


    m_gameObject.getComponent<SpriteRenderComponent>()->IncrementOffset({((int)(elapsedTimeInPhase * 3) % 4 + 1), 0});
    CompleteSpriteChanges(moveVec);
}

void AIController::idleUpdate(float deltaTime)
{
    //do nothing, idle
}


void AIController::moveObj(Vector2f moveVec, float deltaTime)
{
    if (!(moveVec.x == 0 && moveVec.y == 0))
    {
        moveVec.x = moveVec.x / sqrt(moveVec.x * moveVec.x + moveVec.y * moveVec.y);
        moveVec.y = moveVec.y / sqrt(moveVec.x * moveVec.x + moveVec.y * moveVec.y);
    }

    if (m_gameObject.getComponent<RigidBodyComponent>() != nullptr)
    {
        m_gameObject.getComponent<RigidBodyComponent>()->addVelocity(moveVec * speed * deltaTime);
    }
    else
    {
        m_gameObject.move(moveVec * deltaTime * speed);
    }
}


void AIController::RequestNextState()
{
    if (currentState == idle)
    {
        if (CalculateDist() <= 200)
        {
            elapsedTimeInPhase = 0;
            currentState = interested;
        }
    }
    else if (currentState == interested)
    {
        if (CalculateDist() <= 80)
        {
            elapsedTimeInPhase = 0;
            currentState = agressive;
        }
    }
    else if (currentState == agressive)
    {
        if (elapsedTimeInPhase > 2)
        {
            elapsedTimeInPhase = 0;
            currentState = disolve;

        }
    }
    else if (currentState == disolve)
    {
        //m_gameObject.setActive(false); 
        m_gameObject.getComponent<RigidBodyComponent>()->setVelocity({0, 0});
    }
}
void AIController::CompleteSpriteChanges(Vector2f orientation)
{
    Vector2f NewOrientation = Vector2f{-orientation.y, orientation.x};
    auto& sprite = m_gameObject.getComponent<SpriteRenderComponent>().get()->getSprite();
    float angle = atan2(NewOrientation.y, NewOrientation.x);
    angle = angle * 180.0f / static_cast<float>(3.14);


    FloatRect gameObjectSize = m_gameObject.getComponent<RigidBodyComponent>().get()->m_shape;
    Vector2f  spriteOffset   = Vector2f{sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2};


    sprite.setOrigin(spriteOffset);
    sprite.setPosition(Vector2f{gameObjectSize.width / 2.f, gameObjectSize.height / 2.f});
    sprite.setRotation(angle);
}
void AIController::setGoalObject(GameObject* _goal)
{
    m_goal = _goal;
}
AIController::AIController(GameObject& m_gameObject) : IComponent(m_gameObject)
{
    AiManager::getInstance()->registerAIComponent(this);
}
AIController::~AIController()
{
    AiManager::getInstance()->unregisterAIComponent(this);
}
float AIController::CalculateDist()
{
    Vector2f DistVec = m_goal->getPosition() - m_gameObject.getPosition();
    return sqrt(DistVec.x * DistVec.x + DistVec.y * DistVec.y);
}
void AIController::CollisionEvent()
{
    IEvent::Ptr FishColl = std::make_shared<FishCollisionEvent>(1);
    EventBus::getInstance().fireEvent(FishColl);
}
}