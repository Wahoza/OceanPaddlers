//Author: Kilian Kolb
//Description: AI behaviour of the Fish enemy
//License: LICENSE.txt
#pragma once
#include "stdafx.h"

#include "GameObject.hpp"
#include "IComponent.hpp"


//using namespace sf;
namespace mmt_gd
{
class AIController final : public IComponent
{
    using ptr = std::shared_ptr<AIController>;

public:
    enum State
    {
        interested,
        agressive,
        idle,
        dazed,
        disolve
    };

    void setGoalObject(GameObject* _goal);

    void update(float deltaTime) override;

    void agressiveUpdate(float deltaTime);
    void idleUpdate(float deltaTime);
    void dazedUpdate(float deltaTime);
    void interestedUpdate(float deltaTime);
    void disolveUpdate(float deltaTime);

    void moveObj(sf::Vector2f moveVec, float deltaTime);
    void RequestNextState();
    void SetState(State _state);
    void CompleteSpriteChanges(sf::Vector2f orientation);

    AIController(GameObject& _gameObject);
    ~AIController();
    bool init() override;


    float CalculateDist();
    void  CollisionEvent();

    void  TakeDamage();
    State getCurrentState();
    float speed = 100;
    bool attackable = true; //base for fish
    int  healthPoints = 2; //base for fish
    
private:
    State       currentState = idle;
    GameObject* m_goal = nullptr;
    float       elapsedTimeInPhase = 0;
    float       iFrameTime = 0;
    float       iFrameDuration     = 0.15;
};
} // namespace mmt_gd