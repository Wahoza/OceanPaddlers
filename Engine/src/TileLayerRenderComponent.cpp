#include "stdafx.h"

#include "TileLayerRenderComponent.hpp"

#include "EventBus.hpp"
#include "GameObject.hpp"
#include "RenderComponentEvents.hpp"

#include <utility>

namespace mmt_gd
{
TileLayerRenderComponent::TileLayerRenderComponent(GameObject& gameObject, sf::RenderWindow& renderWindow, TileLayer layer) :
IRenderComponent(gameObject, renderWindow),
m_layer(std::move(layer))
{
    EventBus::getInstance().fireEvent(std::make_shared<RenderableCreateEvent>(m_layer.m_name, *this));
}

TileLayerRenderComponent::~TileLayerRenderComponent()
{
    EventBus::getInstance().fireEvent(std::make_shared<RenderableDestroyEvent>(m_layer.m_name, *this));
}

bool TileLayerRenderComponent::init()
{
    return true;
}

void TileLayerRenderComponent::update(float deltaTime)
{
}

void TileLayerRenderComponent::draw()
{
    for (auto& tile : m_layer.m_tiles)
    {
        m_renderWindow.draw(*tile.m_sprite);
    }
}
} // namespace mmt_gd
