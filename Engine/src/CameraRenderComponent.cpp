//Author: Radomir Dinic
//Description: Camera Component for a GameObject, manages View. Used in MainState.
//License: LICENSE.txt
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

sf::View CameraRenderComponent::getLetterboxView(sf::View view, int windowWidth, int windowHeight)
{
    float windowRatio = windowWidth / static_cast<float>(windowHeight);
    float viewRatio   = view.getSize().x / view.getSize().y;

    float sizeX = 1.0f;
    float sizeY = 1.0f;
    float posX  = 0.0f;
    float posY  = 0.0f;

    bool horizontalSpacing = true;

    if (windowRatio < viewRatio)
    {
        horizontalSpacing = false;
        sizeX             = viewRatio / windowRatio;
        posX              = (1.0f - sizeX) / 2.0f;
    }
    else
    {
        sizeY = windowRatio / viewRatio;
        posY  = (1.0f - sizeY) / 2.0f;
    }

    view.setViewport(sf::FloatRect(posX, posY, sizeX * 0.1, sizeY*0.1));

    return view;
}

void CameraRenderComponent::update(float deltaTime)
{
    m_view.setCenter(m_gameObject.getPosition());
    //m_view = getLetterboxView(m_view, m_renderWindow.getSize().x, m_renderWindow.getSize().y);
    m_renderWindow.setView(m_view);
}
} // namespace mmt_gd
