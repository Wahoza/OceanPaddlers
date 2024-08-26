//Author: Kilian Kolb
//Description: Handles Player status and notifies objects with Events
//License: LICENSE.txt
#pragma once
#include "stdafx.h"

#include "IComponent.hpp"
#include "IEvent.hpp"
namespace mmt_gd
{
class PlayerComponent : public IComponent
{
//idea: rather then the two rects of the tether refrencing the tether as a player, they both call this component
public:
    PlayerComponent(GameObject& gameObject);
    void CollectibleCollision(std::string _item); //send Collision Event
    void DamageCollision(); //send Damage Event or GameOver Event
    void GameEnd(); //send GameEnd Event
    void Victory();
    void update(float deltaTime) override;
    void  onEnvironmentSwap(IEvent::Ptr event);
    float getHurtTime();
    float getMaxHurtTime();
    bool init() override;

private:
    int  EggScore     = 0;
    int  initlives    = 3;
    int  currentlives = 3;
    int  totalScore   = 0;
    bool won          = false;

    float distanceTraveled = 0;
    float distancePixeltoM = 30;

    float swapDuration = 0;
    float timeSinceLastSwap = 0;
    
    float ObjectYonSwap = 8 * 16;
    float maxHurtTime   = 5;
    float hurtTime      = maxHurtTime + 1;

    std::vector<float> boundings = {0, 20 * 16};
};

}