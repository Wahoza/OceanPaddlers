#pragma once

#include "IRenderComponent.hpp"

#include <SFML/Graphics.hpp>
#include <string>

namespace mmt_gd
{
class SpriteRenderComponent final : public IRenderComponent
{
public:
    using Ptr = std::shared_ptr<SpriteRenderComponent>;

    SpriteRenderComponent(GameObject& gameObject, sf::RenderWindow& renderWindow, std::string textureName, std::string layerName);

    ~SpriteRenderComponent() override;

    bool init() override;

    void update(float deltaTime) override;

    void draw() override;

    sf::Sprite& getSprite()
    {
        return m_sprite;
    }
    void updateIntRect(sf::Vector2i Iterator);

    void IncrementOffset(sf::Vector2i increment);

    void HideSprite();
    void setSpriteOpacity(float opacity);
    void ShowSprite();

private:
    std::string m_textureFile;
    sf::Texture m_texture;
    sf::Sprite  m_sprite;
    std::string m_layerName;
    sf::Vector2i m_offset;
    bool         show;
};
} // namespace mmt_gd
