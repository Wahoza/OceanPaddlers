#pragma once

#include "stdafx.h"
#include "AIController.hpp"

#include "GameObject.hpp"
#include "GameObjectManager.hpp"
using namespace std;
namespace mmt_gd
{

class AiManager
{
public:
    static AiManager* getInstance();
    void              setGoalObject(GameObject* _goal);
    void              update();
    void registerAIComponent(AIController* _AIcontroller);
    void              shutdown();
    void              unregisterAIComponent(AIController* _AIcontroller);

private:
    std::vector<AIController*> AIcontrollers;
    GameObject* m_goal = nullptr;
    static AiManager* m_instance;
};
} // namespace mm_gd