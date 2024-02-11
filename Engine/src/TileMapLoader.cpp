//Author: Radomir Dinic
//License: LICENSE.txt
#include "stdafx.h"

#include "TileMapLoader.hpp"

#include "GameObjectEvents.hpp"
#include "ObjectFactory.hpp"
#include "TileLayerRenderComponent.hpp"
#include "Tileson.hpp"
#include <iostream>


namespace mmt_gd
{
void TileMapLoader::loadTileSetTextures(const std::unique_ptr<tson::Map>& map,
                                        const fs::path&                   resourcePath,
                                        SpriteManager&                    spriteManager,
                                        SpriteManager::TileSetMap&        tileSets)
{
    for (const auto& tileSet : map->getTilesets())
    {
        std::cout << "Load tileset: " << tileSet.getName() << " width filename: " << tileSet.getImagePath()
                  << " and tile size: " << tileSet.getTileSize().x << ", " << tileSet.getTileSize().y << std::endl;

        const auto texture = std::make_shared<sf::Texture>();

        if (!texture->loadFromFile((resourcePath / tileSet.getImagePath()).string()))
        {
            sf::err() << "Could not load texture " << resourcePath / tileSet.getImagePath() << std::endl;
            continue;
        }
        tileSets[tileSet.getName()] = texture;
    }
    spriteManager.setTileSets(tileSets);
}

void TileMapLoader::createTileSprite(
    const std::unique_ptr<tson::Map>& map,
    SpriteManager::TileSetMap&        tileSets,
    std::vector<TileLayer>&           tileLayers,
    int                               layerIdx,
    const tson::Layer&                layer,
    int                               i,
    const unsigned                    gid,
    SpriteManager                     spriteManager)
{
    // get tileSet and tileSet texture
    const auto* const  tileSet = map->getTilesetByGid(gid);
    const sf::Vector2i tileSize(map->getTileSize().x, map->getTileSize().y);
    const auto&        texture = tileSets[tileSet->getName()];

    assert(texture != nullptr);

    // horizontal tile count in tileSet texture
    const int tileCountX = texture->getSize().x / tileSize.x;
    const int tileCountY = texture->getSize().y / tileSize.y;


    // calculate position of tile
    sf::Vector2f position;
    position.x = i % layer.getSize().x * tileSize.x;
    position.y = i / layer.getSize().x * tileSize.y;

    // calculate 2d idx of tile in tileSet texture
    const auto idx  = gid - tileSet->getFirstgid();
    const auto idxX = idx % tileCountX;
    const auto idxY = idx / tileCountX;

    // calculate source area of tile in tileSet texture
    const sf::IntRect source(idxX * tileSize.x, idxY * tileSize.y, tileSize.x, tileSize.y);

    // create tile and put it into a layer
    auto       mapHeight = map->getSize().y * map->getTileSize().y;
    //auto       scaleFactor = spriteManager.getWindow().getSize().y / mapHeight;
    const auto sprite = std::make_shared<sf::Sprite>();
    sprite->setTexture(*texture);
    sprite->setTextureRect(source);
    sprite->setPosition(position.x , position.y );
    //sprite->setScale(sf::Vector2f(scaleFactor, scaleFactor));

    tileLayers[layerIdx].m_tiles.push_back({i, sprite});
}

void TileMapLoader::loadTileLayers(const std::unique_ptr<tson::Map>& map, const fs::path& resourcePath, SpriteManager& spriteManager)
{
    std::cout << "Load tileMap with size: " << map->getSize().x << "x" << map->getSize().y
              << " and tile size: " << map->getTileSize().x << "x" << map->getTileSize().y << std::endl;

    spriteManager.setTileSize({map->getTileSize().x, map->getTileSize().y});

    SpriteManager::TileSetMap tileSets;
    loadTileSetTextures(map, resourcePath, spriteManager, tileSets);

    // go through all layers
    std::vector<TileLayer> tileLayers;
    tileLayers.resize(map->getLayers().size());

    for (auto layerIdx = 0; layerIdx < static_cast<int>(map->getLayers().size()); layerIdx++)
    {
        tson::Layer layer = map->getLayers()[layerIdx];

        std::cout << "Load layer: " << layer.getName() << " with width: " << layer.getSize().x
                  << " and height: " << layer.getSize().y << std::endl;

        const auto size = layer.getSize().x * layer.getSize().y;

        tileLayers[layerIdx] = TileLayer{layer.getName(),
                                         sf::Vector2i{layer.getSize().x, layer.getSize().y},
                                         sf::Vector2i{map->getTileSize().x, map->getTileSize().y},
                                         {}};

        // go over all elements in the layer
        for (auto i = 0; i < size; i++)
        {
            const auto gid = layer.getData()[i];

            if (gid == 0)
            {
                continue;
            }

            createTileSprite(map, tileSets, tileLayers, layerIdx, layer, i, gid, spriteManager);
        }
    }

    // Add objects to layers
    auto go = GameObject::create("TileMap");
    EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(go));

    for (const auto& layer : tileLayers)
    {
        go->addComponent<TileLayerRenderComponent>(*go, spriteManager.getWindow(), layer);
    }

    if (!go->init())
    {
        sf::err() << "Could not initialize go " << go->getId() << " in TileMap " << std::endl;
    }
}


void TileMapLoader::loadObjectLayers(const std::unique_ptr<tson::Map>& map,
                                     const fs::path&                   resourcePath,
                                     const SpriteManager&              spriteManager)
{
    // go through all object layers
    for (auto& layer : map->getLayers())
    {
        // go over all objects per layer and construct them
        for (auto& object : layer.getObjects())
        {
            ObjectFactory::processTsonObject(object, layer, resourcePath, spriteManager);
            
        }
    }
}
} // namespace mmt_gd
