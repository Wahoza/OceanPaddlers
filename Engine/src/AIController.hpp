#pragma once
#include "stdafx.h"
#include "IComponent.hpp"
#include "GameObject.hpp"

using namespace sf;
namespace mmt_gd
{
class AIController : public IComponent
{
    using ptr = std::shared_ptr<AIController>;

public:
    enum State
    {
        defensive,
        agressive,
        idle
    };

    void GetGoalObject(GameObject* _ball);

    void update(float deltaTime) override;

    void agressiveUpdate(float deltaTime);
    void idleUpdate(float deltaTime);
    void defensiveUpdate(float deltaTime);

    void moveObj(Vector2f moveVec, float deltaTime);
    void RequestNextState();

    AIController(GameObject& gameObject);
    bool init() override;

private:
    State       currentState = idle;
    GameObject* m_ball;
    float       elapsedTimeInPhase = 0;
};
} // namespace mmt_gd