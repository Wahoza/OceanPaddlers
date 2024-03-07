#pragma once
#include "stdafx.h"
#include "SpriteManager.hpp"
#include "Tileson.hpp"

#include "BackGroundDetailGen.hpp"

namespace mmt_gd
{
class EndlessBackgroundGenerator
{
public:
    using TileSetMap = std::unordered_map<std::string, std::shared_ptr<sf::Texture>>;

    void Initialize();
    void loadTileSetTextures(const std::unique_ptr<tson::Map>& map,
                             const fs::path&                   resourcePath,
                             SpriteManager&                    spriteManager,
                             TileSetMap&        tileSets);

    void createTileSprite(const std::unique_ptr<tson::Map>& map,
                          TileSetMap&        tileSets,
                          int                               layerIdx,
                          const tson::Layer&                layer,
                          int                               i,
                          const unsigned                    gid);

    void loadTileLayers(const std::unique_ptr<tson::Map>& map, const fs::path& resourcePath, SpriteManager& spriteManager);
    void Organize();
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    void shutdown();

private:
    std::vector<std::shared_ptr<sf::Sprite>> sprites;
    std::vector<std::vector<sf::Sprite*>>    OrganizedSprites;
    sf::FloatRect                        m_lastWindowRect;
    TileSetMap                               m_tileSets;
    BackgroundDetailGen                      m_DetailGen;
};
}