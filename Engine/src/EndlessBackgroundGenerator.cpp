#include "stdafx.h"
#include "EndlessBackgroundGenerator.hpp"
#include "BackGroundDetailGen.hpp"

#include "Tileson.hpp"
#include "SpriteManager.hpp"
namespace mmt_gd
{
void EndlessBackgroundGenerator::Initialize()
{
}
void EndlessBackgroundGenerator::loadTileSetTextures(const std::unique_ptr<tson::Map>& map,
                                        const fs::path&                   resourcePath,
                                        SpriteManager&                    spriteManager,
                                        TileSetMap&        tileSets)
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
    m_tileSets = tileSets;
}
void EndlessBackgroundGenerator::createTileSprite(
    const std::unique_ptr<tson::Map>& map,
    SpriteManager::TileSetMap&        tileSets,
    int                               layerIdx,
    const tson::Layer&                layer,
    int                               i,
    const unsigned                    gid)
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
    auto mapHeight = map->getSize().y * map->getTileSize().y;
    //auto       scaleFactor = spriteManager.getWindow().getSize().y / mapHeight;
    const auto sprite = std::make_shared<sf::Sprite>();
    sprite->setTexture(*texture);
    sprite->setTextureRect(source);
    sprite->setPosition(position.x, position.y);

    sprites.push_back(sprite);
}
void EndlessBackgroundGenerator::loadTileLayers(const std::unique_ptr<tson::Map>& map, const fs::path& resourcePath, SpriteManager& spriteManager)
{
    std::cout << "Load tileMap with size: " << map->getSize().x << "x" << map->getSize().y
              << " and tile size: " << map->getTileSize().x << "x" << map->getTileSize().y << std::endl;

    //spriteManager.setTileSize({map->getTileSize().x, map->getTileSize().y});

    TileSetMap tileSets;
    loadTileSetTextures(map, resourcePath, spriteManager, tileSets);

    auto layerIdx = 0;
    tson::Layer layer    = map->getLayers()[layerIdx];

    const auto size     = layer.getSize().x * layer.getSize().y;

    for (auto i = 0; i < size; i++)
    {
        const auto gid = layer.getData()[i];

        if (gid == 0)
        {
            continue;
        }
        createTileSprite(map, tileSets, layerIdx, layer, i, gid);

    }

    Organize();

    m_DetailGen.init(sprites[0].get()->getTextureRect(), {int(OrganizedSprites.size()), int(OrganizedSprites[0].size())});
}
void EndlessBackgroundGenerator::Organize()
{
    int height = 17;
    int width  = sprites.size() / height;

    for (int g = width; g >0; g--)
    {
        std::vector<sf::Sprite*> subVector;

        for (int i = g; i < sprites.size(); i += width)
        {
            //std::cout << sprites[i]->getPosition().x << std::endl;
            subVector.push_back(sprites[i].get());
        }

        OrganizedSprites.push_back(subVector);
    }
}
void EndlessBackgroundGenerator::update(float deltaTime)
{
    if (OrganizedSprites[OrganizedSprites.size() - 1][0]->getPosition().x <
        m_lastWindowRect.left - OrganizedSprites[OrganizedSprites.size() - 1][0]->getTextureRect().width)
    {

        std::vector<sf::Sprite*> poppedElement = OrganizedSprites.back();
        OrganizedSprites.pop_back();
        //std::cout << std::to_string(OrganizedSprites[OrganizedSprites.size() - 1][0]->getPosition().x) + " sprite"
        //          << std::endl;

        for (auto& tile : poppedElement)
        {
            tile->setPosition(OrganizedSprites[1][0]->getPosition().x + tile->getTextureRect().getSize().x,
                              tile->getPosition().y);
        }
        // Insert the popped element at the beginning
        OrganizedSprites.insert(OrganizedSprites.begin(), poppedElement);

        //std::cout << std::to_string(OrganizedSprites[0][0]->getPosition().x) + " relocate"
        //          << std::endl;
        //std::cout << std::to_string(m_lastWindowRect.left) + " window" << std::endl;
    }
    m_DetailGen.update(deltaTime, m_lastWindowRect);
}
void EndlessBackgroundGenerator::draw(sf::RenderWindow& window)
{
    m_lastWindowRect = {
        window.getView().getCenter().x - window.getView().getSize().x/2,
        window.getView().getCenter().y - window.getView().getSize().y / 2,
        window.getView().getSize().x,
        window.getView().getSize().y
    };
    for (auto& TileY : OrganizedSprites)
    {
        for (auto& Tile : TileY)
        {
            window.draw(*Tile);
        }
    }
    m_DetailGen.draw(window);
}
} // namespace mmt_gd