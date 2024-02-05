#include "stdafx.h"

#include "AssetManager.hpp"
namespace mmt_gd
{
AssetManager& AssetManager::getInstance()
{
    static AssetManager instance;
    return instance;
}
void AssetManager::init()
{
    addTexture("../assets/sprites/egg.png", "Egg");
    addTexture("../assets/sprites/turtles.png", "Turtles");
    addTexture("../assets/sprites/Anleitung2.png", "Anleitung");
    addTexture("../assets/sprites/pushfish.png", "Fish");
}
sf::Texture& AssetManager::getTexture(std::string textureName)
{
    return *m_textures[textureName];
}
void AssetManager::addTexture(std::string filePath, std::string textureName)
{
    auto texture = std::make_shared<sf::Texture>();

    if (!texture->loadFromFile(filePath)){

        return;
    }
    m_textures[textureName] = texture;
}
}