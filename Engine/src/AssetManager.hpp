#pragma once
#include "stdafx.h"

#include <unordered_map>
namespace mmt_gd
{
class AssetManager
{
public:
    static AssetManager& getInstance();

    AssetManager(const AssetManager& rhv)             = delete;
    AssetManager(AssetManager&& rhv)                  = delete;
    AssetManager&  operator=(const AssetManager& rhv) = delete;
    AssetManager&& operator=(AssetManager&& rhv)      = delete;

    AssetManager()  = default;
    ~AssetManager() = default;

    void init();

    sf::Texture& getTexture(std::string textureName);

    void addTexture(std::string filePath, std::string textureName);

private:
    std::unordered_map<std::string, std::shared_ptr<sf::Texture>> m_textures;
};
} // namespace mmt_gd