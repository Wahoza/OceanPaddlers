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

    SpriteRenderComponent(GameObject& gameObject, sf::RenderWindow& renderWindow, std::string textureFile, std::string layerName);

    ~SpriteRenderComponent() override;

    bool init() override;

    void update(float deltaTime) override
    {
    }

    void draw() override;

    sf::Sprite& getSprite()
    {
        return m_sprite;
    }
    

private:
    std::string m_textureFile;
    sf::Texture m_texture;
    sf::Sprite  m_sprite;
    std::string m_layerName;
};
} // namespace mmt_gd
