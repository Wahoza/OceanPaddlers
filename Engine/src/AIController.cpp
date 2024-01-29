#include "stdafx.h"
#include "AIController.hpp"
#include "GameObject.hpp"
#include "RigidBodyComponent.hpp";
namespace mmt_gd
{
void AIController::update(const float deltaTime)
{
    RequestNextState();
    elapsedTimeInPhase += deltaTime;

    switch (currentState)
    {
        case idle:
            //std::cout << "idle" << std::endl;

            idleUpdate(deltaTime);
            break;
        case defensive:
            //std::cout << "def" << std::endl;

            defensiveUpdate(deltaTime);
            break;
        case agressive:
            //std::cout << "agr" << std::endl;

            agressiveUpdate(deltaTime);
            break;
    }
}
bool AIController::init()
{
    return true;
}
void AIController::agressiveUpdate(float deltaTime)
{
    Vector2f moveVec = m_ball->getPosition() - m_gameObject.getPosition();

    moveObj(moveVec, deltaTime);
}
void AIController::defensiveUpdate(float deltaTime)
{
    Vector2f moveVec{};
    moveVec.y = m_ball->getPosition().y - m_gameObject.getPosition().y;
    moveVec.x = 2240 - m_gameObject.getPosition().x;

    moveObj(moveVec, deltaTime);
}

void AIController::idleUpdate(float deltaTime)
{
    //do nothing, easier for player to win
}
void AIController::moveObj(Vector2f moveVec, float deltaTime)
{
    moveVec = moveVec / sqrt(moveVec.x * moveVec.x + moveVec.y * moveVec.y);
    if (m_gameObject.getComponent<RigidBodyComponent>() != nullptr)
    {
        m_gameObject.getComponent<RigidBodyComponent>()->addVelocity(moveVec);
    }
    else
    {
        //note: still needs to be normalized
        m_gameObject.move(moveVec * deltaTime);
    }
}


void AIController::RequestNextState()
{
  
    if (elapsedTimeInPhase >= 4)
    {
        //std::cout << "new" << std::endl;
        elapsedTimeInPhase = 0;
        float Xdist        = (m_ball->getPosition().x - m_gameObject.getPosition().x);
        float Ydist                 = (m_ball->getPosition().y - m_gameObject.getPosition().y);
        float DistanceToGoal = sqrt(Xdist * Xdist + Ydist * Ydist);
        
        if (std::rand() % 100 < 5)
        {
            currentState = idle;
            
        }
        else
        {
            if (DistanceToGoal >= 800 || std::rand() % 100 + 1 < 10)
            {
                currentState = defensive;
            }
            else
            {
                currentState = agressive;
            }
        }

    }
}
void AIController::GetGoalObject(GameObject* _ball)
{
    m_ball = _ball;
}
AIController::AIController(GameObject& gameObject) : IComponent(gameObject)
{
}

}