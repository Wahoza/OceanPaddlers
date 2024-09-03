#pragma once
#include "stdafx.h"
#include "IComponent.hpp"
#include "TriggerComponent.hpp"
#include "IEvent.hpp"
namespace mmt_gd
{
class PlayerAttackComponent : public IComponent
{
public:
    PlayerAttackComponent(GameObject& gameObject, sf::Vector2f size, float expansion, float HitboxScale, int PlayerID);
    bool init() override;
    void update(const float deltaTime) override;
    
private:
    //Trigger, Offset
    void ListenPowerupState(IEvent::Ptr event);
    std::vector<std::tuple<TriggerComponent*, Vector2f>> triggers;
    void     RealignTriggers(std::vector<std::tuple<TriggerComponent*, Vector2f>> _triggers);
    Vector2f RotateVec(Vector2f input, float angle);
    void     DoAttackFrame(bool active);
    int  pID = 0;
    float attackCooldown = 0.2;
    float attackDuration = 0.1;
    float attackTimer    = -1;
    float    directionThreshold = 0.5;
    bool     attacking          = false;
    bool     poweredUp = false;
};
}