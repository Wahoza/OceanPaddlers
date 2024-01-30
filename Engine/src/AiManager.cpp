#include "stdafx.h"
#include "AiManager.hpp"
#include "AIController.hpp"


namespace mmt_gd
{
AiManager* AiManager::m_instance;
AiManager* AiManager::getInstance()
{

    if (m_instance == nullptr)
    {
        m_instance = new AiManager();
    }
    return m_instance;
}
void AiManager::registerAIComponent(AIController* _AIcontroller)
{
    _AIcontroller->setGoalObject(m_instance->m_goal);
    m_instance->AIcontrollers.push_back(_AIcontroller);
    std::cout << "Added AI to Manager" << std::endl;
}
void AiManager::setGoalObject(GameObject* _goal)
{
    m_instance->m_goal = _goal;

    if (m_instance->AIcontrollers.size() == 0)
        return;
    for (auto& AIcomponent : m_instance->AIcontrollers)
    {
        AIcomponent->setGoalObject(_goal);
    }
}
}