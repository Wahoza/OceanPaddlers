#include "stdafx.h"
#include "EndlessBackgroundGenerator.hpp"
#include "BackGroundDetailGen.hpp"
#include "IEvent.hpp"
#include "GameObjectEvents.hpp"
#include "EventBus.hpp"
#include "Tileson.hpp"
#include "SpriteManager.hpp"
namespace mmt_gd
{

void EndlessBackgroundGenerator::loadTileSetTextures(const std::unique_ptr<tson::Map>& map,
                                        const fs::path&                   resourcePath,
                                        SpriteManager&                    spriteManager,
                                        TileSetMap&        tileSets)
{
    for (const auto& tileSet : map->getTilesets())
    {
        /*std::cout << "Load tileset: " << tileSet.getName() << " width filename: " << tileSet.getImagePath()
                  << " and tile size: " << tileSet.getTileSize().x << ", " << tileSet.getTileSize().y << std::endl;*/
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
void EndlessBackgroundGenerator::init(const std::unique_ptr<tson::Map>& map, const fs::path& resourcePath, SpriteManager& spriteManager)
{
    _map = map.get();
    /*std::cout << "Load tileMap with size: " << map->getSize().x << "x" << map->getSize().y
              << " and tile size: " << map->getTileSize().x << "x" << map->getTileSize().y << std::endl;*/

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
    tileSize = sprites[0]->getTextureRect().getSize().x;
    LoadFromFolder("grassTiles/");

     if (isInit)
    {
        return;
    }
    isInit = true;
    EventBus::getInstance().addListener(PowerUP::Type, [this](IEvent::Ptr event) { PwrUPListener(event); });
}
void EndlessBackgroundGenerator::Organize()
{
    bool isFirstZero = true;

    int height = 17;
    width  = (sprites.size()) / height;

    for (int g = width; g >= 0; g--)
    {
        std::vector<sf::Sprite*> subVector;

        for (int i = g; i < sprites.size(); i += width)
        {
            //std::cout << sprites[i]->getPosition().x << std::endl;
            subVector.push_back(sprites[i].get());
        }
        if (!(subVector[0]->getPosition().x == 0 && isFirstZero))
        {
            OrganizedSprites.push_back(subVector);
        }
        else if (isFirstZero)
        {
            isFirstZero = false;
        }
    }
}
void EndlessBackgroundGenerator::RemoveUselessTiles()
{
    for (auto& SubVec : OrganizedSprites)
    {
        int iterator = 0;
        for (auto& tile = SubVec.begin(); tile != SubVec.end();)
        {
            if (iterator > 4 && iterator < 13)
            {
                tile = SubVec.erase(tile);
                ++iterator;

                continue;
            }
            ++iterator;
            ++tile;
        }
    }
}
void EndlessBackgroundGenerator::update(float deltaTime, sf::FloatRect lastWindowRect)
{
    //LoadFromFolder("");
    m_lastWindowRect = lastWindowRect;
    if (OrganizedSprites.back()[0]->getPosition().x < m_lastWindowRect.left - tileSize * 2) //x2 for buffer tile
    {

        std::vector<sf::Sprite*> poppedElement = OrganizedSprites.back();
        OrganizedSprites.pop_back();
        //std::cout << std::to_string(OrganizedSprites[OrganizedSprites.size() - 1][0]->getPosition().x) + " sprite"
        //          << std::endl;

        for (auto& tile : poppedElement)
        {
            tile->setPosition(OrganizedSprites.front()[0]->getPosition().x + tileSize, tile->getPosition().y);
        }
        // Insert the popped element at the beginning
        OrganizedSprites.insert(OrganizedSprites.begin(), poppedElement);

        //std::cout << std::to_string(OrganizedSprites[0][0]->getPosition().x) + " relocate"
        //          << std::endl;
        //std::cout << std::to_string(m_lastWindowRect.left) + " window" << std::endl;
    }
    //m_DetailGen.update(deltaTime, m_lastWindowRect);
}
void EndlessBackgroundGenerator::PwrUPListener(IEvent::Ptr event)
{
    std::cout << isPowerUP;

    if (auto PowerUp = std::dynamic_pointer_cast<PowerUP>(event))
    {
        isPowerUP = PowerUp.get()->getData();
    }
}
void EndlessBackgroundGenerator::draw(sf::RenderWindow& window)
{
    if (!isPowerUP)
    {
        window.clear(clearColor);
    }
    else
    {
        window.clear(sf::Color::Black);
    }
    for (auto& Tile : sprites)
    {
        float ySpritePos = Tile.get()->getPosition().y;

        if (3 * tileSize < ySpritePos && ySpritePos < 13 * tileSize)
        {
            sf::RectangleShape testRect;
            testRect.setFillColor({0, 0, 0, 255});
            testRect.setScale(tileSize, tileSize);
            testRect.setPosition(Tile.get()->getPosition());
            window.draw(testRect);

        }
        else
        {
            window.draw(*Tile);
        }
    }
}
void EndlessBackgroundGenerator::LoadFromFolder(std::string FolderName)
{
    
    for (auto& tileSet : m_tileSets)
    {   
        std::cout << tileSet.first << std::endl;
        sf::Texture tempTexture;
        
        if (!tempTexture.loadFromFile("../assets/SegmentedTiles/" + FolderName + tileSet.first + ".png"))
        {
            continue;
        }
        sf::Image tempImage;
        tempImage.loadFromFile("../assets/SegmentedTiles/" + FolderName + tileSet.first + ".png");

        tileSet.second->swap(tempTexture);
        tileSet.second->setSmooth(false);

        clearColor = tempImage.getPixel(tileSize * 1.5, tileSize * 2.5);
    }
}
void EndlessBackgroundGenerator::shutdown()
{
    OrganizedSprites = {};
    sprites          = {};
}
sf::Vector2i EndlessBackgroundGenerator::GetTotalSize()
{
    return {int(OrganizedSprites.size()), int(OrganizedSprites[0].size())};
}
sf::Vector2i EndlessBackgroundGenerator::GetTileSize()
{
    return {int(sprites[0]->getTextureRect().width), int(sprites[0]->getTextureRect().height)};
}
} // namespace mmt_gd