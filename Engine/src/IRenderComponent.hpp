#pragma once

#include "IComponent.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

namespace mmt_gd
{
class IRenderComponent : public IComponent
{
public:
    using Ptr     = std::shared_ptr<IRenderComponent>;
    using WeakPtr = std::weak_ptr<IRenderComponent>;

    IRenderComponent(GameObject& gameObject, sf::RenderWindow& renderWindow) :
    IComponent(gameObject),
    m_renderWindow(renderWindow)
    {
    }

    virtual void draw() = 0;

protected:
    sf::RenderWindow& m_renderWindow;
};
} // namespace mmt_gd
