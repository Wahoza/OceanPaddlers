//Author: Kilian Kolb
//Description: Handles Player status and notifies objects with Events
//License: LICENSE.txt
#pragma once
#include "stdafx.h"

#include "IComponent.hpp"

namespace mmt_gd
{
class PlayerComponent : public IComponent
{
//idea: rather then the two rects of the tether refrencing the tether as a player, they both call this component
public:
    PlayerComponent(GameObject& gameObject);
    void CollectibleCollision(); //send Collision Event
    void DamageCollision(); //send Damage Event or GameOver Event
    void GameEnd(); //send GameEnd Event
    void Victory();
    void update(float deltaTime) override;
    float getHurtTime();
    bool init() override;

private:
    int  EggScore     = 0;
    int  initlives    = 3;
    int  currentlives = 3;
    bool won          = false;

    float hurtTime = 0;

    float distanceTraveled = 0;
    float distancePixeltoM = 30;
};

}