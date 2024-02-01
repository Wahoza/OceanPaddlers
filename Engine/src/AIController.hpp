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
        idle
    };

    void setGoalObject(GameObject* _goal);

    void update(float deltaTime) override;

    void agressiveUpdate(float deltaTime);
    void idleUpdate(float deltaTime);
    void interestedUpdate(float deltaTime);

    void moveObj(sf::Vector2f moveVec, float deltaTime);
    void RequestNextState();
    void CompleteSpriteChanges(sf::Vector2f orientation);
    AIController(GameObject& _gameObject);
    ~AIController();
    bool init() override;

    float CalculateDist();
    void  CollisionEvent();
    float speed = 100;

private:
    State       currentState = idle;

    GameObject* m_goal = nullptr;
    float       elapsedTimeInPhase = 0;
};
} // namespace mmt_gd