#include "stdafx.h"

#include "SpriteRenderComponent.hpp"

#include "EventBus.hpp"
#include "GameObject.hpp"
#include "RenderComponentEvents.hpp"

#include <utility>

namespace mmt_gd
{
SpriteRenderComponent::SpriteRenderComponent(GameObject&       gameObject,
                                             sf::RenderWindow& renderWindow,
                                             std::string       textureFile,
                                             std::string       layerName) :
IRenderComponent(gameObject, renderWindow),
m_textureFile(std::move(textureFile)),
m_layerName(std::move(layerName))
{
    EventBus::getInstance().fireEvent(std::make_shared<RenderableCreateEvent>(m_layerName, *this));
}
void SpriteRenderComponent::update(float deltaTime)
{
    updateIntRect(m_offset);
    m_offset = {0, 0};
}
SpriteRenderComponent::~SpriteRenderComponent()
{
    EventBus::getInstance().fireEvent(std::make_shared<RenderableDestroyEvent>(m_layerName, *this));
}
void SpriteRenderComponent::updateIntRect(sf::Vector2i Iterator)
{
    sf::IntRect newRect = m_sprite.getTextureRect();
    newRect.top     = newRect.height * Iterator.y;
    newRect.left    = newRect.width * Iterator.x;
    m_sprite.setTextureRect(newRect);
}
void SpriteRenderComponent::IncrementOffset(sf::Vector2i increment)
{
    m_offset += increment;
}
bool SpriteRenderComponent::init()
{
    sf::Image image;
    if (!image.loadFromFile(m_textureFile))
    {
        sf::err() << "Could not load texture from " << m_textureFile << std::endl;
        return false;
    }
    //image.createMaskFromColor(sf::Color::Black);
    m_texture.loadFromImage(image);
    m_texture.setSmooth(false);
    m_sprite.setTexture(m_texture);

    ShowSprite();

    return true;
}
void SpriteRenderComponent::HideSprite()
{
    show = false;
}
void SpriteRenderComponent::setSpriteOpacity(float opacity)
{
    auto spriteColor = m_sprite.getColor();
    spriteColor.a    = opacity;
    m_sprite.setColor(spriteColor);
}
void SpriteRenderComponent::ShowSprite()
{
    show = true;
}
void SpriteRenderComponent::draw()
{
    if (show)
    {
        m_renderWindow.draw(m_sprite, m_gameObject.getTransform());
    }
}
} // namespace mmt_gd
