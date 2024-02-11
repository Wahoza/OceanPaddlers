//Author: Radomir Dinic
//Description: Base Class for a GameObject. GameObjects consist of multiple Components (IComponent/IRenderComponent).
//License: LICENSE.txt
#include "stdafx.h"

#include "GameObject.hpp"

namespace mmt_gd
{
void GameObject::removeComponent(const IComponent::Ptr& component)
{
    const auto it = std::find(m_componentList.begin(), m_componentList.end(), component);
    if (it != m_componentList.end())
    {
        m_componentList.remove(component);
        delete &component;
    }
}

void GameObject::update(const float deltaTime) const
{
    for (const auto& comp : m_componentList)
    {
        comp->update(deltaTime);
    }
}

bool GameObject::init() const
{
    for (const auto& comp : m_componentList) // NOLINT(readability-use-anyofallof)
    {
        if (!comp->init())
        {
            sf::err() << "Could not initialize component of object " << m_id << std::endl;
            return false;
        }
    }
    return true;
}
} // namespace mmt_gd
