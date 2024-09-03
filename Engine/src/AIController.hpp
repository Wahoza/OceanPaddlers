//Author: Kilian Kolb
//Description: AI behaviour of the Fish enemy
//License: LICENSE.txt
#pragma once
#include "stdafx.h"

#include "GameObject.hpp"
#include "IComponent.hpp"
#include "CollisionParticleHandler.hpp"

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
        disolve,
        killed
    };

    void setGoalObject(GameObject* _goal);

    void update(float deltaTime) override;

    void agressiveUpdate(float deltaTime);
    void idleUpdate(float deltaTime);
    void dazedUpdate(float deltaTime);
    void killedUpdate(float deltaTime);
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
    float               maxInterestedTime    = 4;
    sf::Vector2f        ParticleSize         = {2, 1};
    float           ParticleoutlineSize  = 0.5;
    sf::Color           ParticleoutlineColor = {220, 117, 77, 255};
    sf::Color           ParticleColor        = {208, 54, 77, 255};
    float           ParticleSpeed        = 5;
    float           ParticleTTL          = 0.5;
    float           ParticleRadius       = 3;
};
} // namespace mmt_gd