#include "stdafx.h"

#include "CameraRenderComponent.hpp"

#include "GameObject.hpp"

namespace mmt_gd
{
CameraRenderComponent::CameraRenderComponent(GameObject& gameObject, sf::RenderWindow& renderWindow, sf::View view) :
IRenderComponent(gameObject, renderWindow),
m_view(view)
{
}

bool CameraRenderComponent::init()
{
    return true;
}

void CameraRenderComponent::draw()
{
}

void CameraRenderComponent::update(float deltaTime)
{
    m_view.setCenter(m_gameObject.getPosition());
    m_view.setSize(1920, 1080);
    m_renderWindow.setView(m_view);
}
} // namespace mmt_gd
