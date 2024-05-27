#include "stdafx.h"
#include "BackGroundDetailGen.hpp"
#include "EventBus.hpp"
#include "IEvent.hpp"
#include "GameObjectEvents.hpp"

namespace mmt_gd
{
void mmt_gd::BackgroundDetailGen::init(sf::IntRect _TextureRect, sf::Vector2i Area, int seed)
{
    TileLength = _TextureRect.width;

    m_seed = seed;

    EventBus::getInstance().addListener(IslandTileGenEvent::Type, [this](IEvent::Ptr event) { IslandGenListener(event); });

    if (!m_texture.loadFromFile("../assets/noiseAssets.png"))
    {
        sf::err() << "Could not load icon\n";
        return;
    }

    noise.SetSeed(m_seed);
    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);

    noiseLocal.SetSeed(m_seed);
    noiseLocal.SetNoiseType(FastNoiseLite::NoiseType_Value);

    for (int i = 0; i < m_texture.getSize().x / 16; i++)
    {
        std::shared_ptr<sf::Sprite> t_sprite = std::make_shared<sf::Sprite>();

        t_sprite->setTexture(m_texture);
        t_sprite->setTextureRect({i * _TextureRect.width, 0, _TextureRect.width, _TextureRect.height});
        m_SpriteMap[i] = t_sprite;
    }
    //generation stuff here
    for (int x = Area.x; x >= 0 ; x--)
    {
        std::vector<NoisePositionValue> SubVector;

        for (int y = 0; y <= Area.y; y++)
        {
            NoisePositionValue NPV;
            NPV.noise                   = noise.GetNoise(float(x * scaleVal), float(y * scaleVal));
            NPV.localNoise = noiseLocal.GetNoise(float(x * scaleVal*20), float(y * scaleVal*20));
            NPV.pos   = {float(x * 16), float(y * 16)};
            if (NPV.pos.y / TileLength < 14 && NPV.pos.y / TileLength > 2)
            {
                NPV.draw = false;
            }
            else
            {
                NPV.draw = true;
            }
            SubVector.push_back(NPV);
            /*std::cout << "noise: " + std::to_string(NPV.noise) + " pos: " + std::to_string(NPV.pos.x) + " " +
                             std::to_string(NPV.pos.y)
                      << std::endl;*/
        }
        m_NoisePosVals.push_back(SubVector);
    }
    
    initSpriteMap(m_SpriteMapField, "grassTiles/FieldAssets.png");
    initSpriteMap(m_SpriteMapForrest, "grassTiles/ForrestAssets.png");
    initSpriteMap(m_SpriteMapMines, "grassTiles/MineAssets.png");
    initSpriteMap(m_SpriteMapVillage, "grassTiles/VillageAssets.png");

    //m_IslandTiles.push_back({30*16,5*16 });
}
void BackgroundDetailGen::update(float deltaTime, sf::FloatRect windowRect)
{
    if (m_NoisePosVals[m_NoisePosVals.size() - 1][0].pos.x <
        windowRect.left - TileLength)
    {
        m_NoisePosVals.pop_back();
        std::vector<NoisePositionValue> SubVector = m_NoisePosVals[1];
        for (auto& NPV : SubVector)
        {
            NPV.pos.x += TileLength * 2;
            NPV.noise = noise.GetNoise(NPV.pos.x / TileLength * scaleVal, NPV.pos.y / TileLength * scaleVal);
            NPV.localNoise = noiseLocal.GetNoise(NPV.pos.x / TileLength * scaleVal * 20, NPV.pos.y / TileLength * scaleVal * 20);

            if (NPV.pos.y / TileLength < 14 && NPV.pos.y / TileLength > 2)
            {
                NPV.draw = false;
            }
            else
            {
                NPV.draw = true;
            }
        }

        m_NoisePosVals.insert(m_NoisePosVals.begin(), SubVector);
    }
    for (sf::Vector2f tile : m_IslandTiles)
    {
        if (tile.x + TileLength < windowRect.left)
        {
            m_IslandTiles.erase(std::remove(m_IslandTiles.begin(), m_IslandTiles.end(), tile), m_IslandTiles.end());
        }
    }
}
void BackgroundDetailGen::draw(sf::RenderWindow& window)
{
    for (auto& obj : m_NoisePosVals)
    {
        for (NoisePositionValue& NPV : obj)
        {
            if (!NPV.draw)
            {
                continue;
            }

            //note: these are values tuned by testing
            sf::Sprite t_sprite;
            if (NPV.pos.y > 16 * 2 && NPV.pos.y < 16 * 14)
            {
                if (NPV.noise == 0)
                {
                    if (int(NPV.localNoise * 100) % 5 == 0)
                    {
                        t_sprite = *m_SpriteMapVillage[NoiseToInt(m_SpriteMapVillage.size(), float(NPV.localNoise))];
                    }
                    else
                    {
                        t_sprite = *m_SpriteMapForrest[NoiseToInt(m_SpriteMapForrest.size(), float(NPV.localNoise))];
                    }

                    if (NPV.pos.y + 1 > 16 * 13)
                    {
                        t_sprite.setPosition(NPV.pos + sf::Vector2f{0, -3});
                    }
                    else
                    {
                        t_sprite.setPosition(NPV.pos + sf::Vector2f{0, -6});
                    }
                    window.draw(t_sprite);
                }
                else
                {
                    t_sprite = *m_SpriteMapMines[NoiseToInt(m_SpriteMapMines.size(), float(NPV.localNoise))];
                    t_sprite.setPosition(NPV.pos + sf::Vector2f{0, -7});

                    window.draw(t_sprite);
                }

                continue;
            }

            //Conditionals for drawing on land
            if (NPV.noise < -0.3)
            {
                 t_sprite = *m_SpriteMapForrest[NoiseToInt(m_SpriteMapForrest.size(), float(NPV.localNoise))];
            }
            else if (NPV.noise < -0.1)
            {
                if (int(NPV.localNoise * 100) % 3 > 1)
                {
                    continue;
                }
                t_sprite = *m_SpriteMapField[NoiseToInt(m_SpriteMapField.size(), float(NPV.localNoise))];
            }
            else if (NPV.noise < 0.1)
            {
                if (int(NPV.localNoise * 100) % 3 != 0)
                {
                    if (int(NPV.localNoise * 100) % 3 > 1)
                    {
                        continue;
                    }

                    t_sprite = *m_SpriteMapField[NoiseToInt(m_SpriteMapField.size(), float(NPV.localNoise))];
                }
                else
                {
                    t_sprite = *m_SpriteMapVillage[NoiseToInt(m_SpriteMapVillage.size(), float(NPV.localNoise))];
                }
            }
            else if (NPV.noise < 0.8)
            {
                if (int(NPV.localNoise * 100) % 3 > 1)
                {
                    continue;
                }
                t_sprite = *m_SpriteMapField[NoiseToInt(m_SpriteMapField.size(), float(NPV.localNoise))];
            }
            else if (NPV.noise <= 1)
            {
                t_sprite = *m_SpriteMapMines[NoiseToInt(m_SpriteMapMines.size(), float(NPV.localNoise))];
            }
            t_sprite.setPosition(NPV.pos);

            window.draw(t_sprite);
        }
    }
}
void BackgroundDetailGen::IslandGenListener(IEvent::Ptr event)
{
    if (auto IslandEvent = std::dynamic_pointer_cast<IslandTileGenEvent>(event))
    {
        auto& Islands = IslandEvent.get()->getData();

        for (int i = 3; i < m_NoisePosVals[1].size()-3; i++)
        {
            if (!(i < Islands.size() || i>0))
            {
                break;
            }
            else
            {
                if (!m_NoisePosVals[0][i].draw)
                {
                    if (Islands[i] > -1)
                    {
                        m_NoisePosVals[0][i].draw = true;
                        m_NoisePosVals[0][i].noise = Islands[i];

                    }
                    else
                    {
                        m_NoisePosVals[0][i].draw = false;
                    }
                    //m_NoisePosVals[0][i].pos -= {0, 1};
                }
            }
        }
    }
}
void BackgroundDetailGen::initSpriteMap(std::unordered_map<int, std::shared_ptr<sf::Sprite>>& map, std::string FileName)
{
    //note: these tiles can Overlap for larger tiles (eg. the windmill.) this is why the origin of the sprite will be recentered. SpriteSheets ALWAYS need to be two tiles high.
    std::shared_ptr<sf::Texture> texture = std::make_shared<sf::Texture>();
    if (!texture->loadFromFile("../assets/" + FolderName + FileName))
    {
        sf::err() << "Could not load File\n";
        return;
    }

    m_textures.push_back(texture);
    int size = texture->getSize().x / 16;
    
    map.clear();
    for (int i = 0; i < size; i++)
    {
        std::shared_ptr<sf::Sprite> t_sprite = std::make_shared<sf::Sprite>();
        t_sprite->setTexture(*texture);
        t_sprite->setTextureRect({i * 16, 0, 16, int(texture->getSize().y)});
        t_sprite->setOrigin({0.f, float(texture->getSize().y / 2)});

        map[i] = t_sprite;
    }
}
int BackgroundDetailGen::NoiseToInt(int TotalSize, float noiseVal)
{
    float almostmaxVal = float(TotalSize) - 0.01;
    noiseVal   = (noiseVal + 1) / 2; //Noise now is between 0 and 1;
    
    return int(almostmaxVal * noiseVal);
}
void BackgroundDetailGen::LoadTextures(std::string FolderName)
{
    m_textures.clear();
    initSpriteMap(m_SpriteMapField, FolderName + "FieldAssets.png");
    initSpriteMap(m_SpriteMapForrest, FolderName + "ForrestAssets.png");
    initSpriteMap(m_SpriteMapMines, FolderName + "MineAssets.png");
    initSpriteMap(m_SpriteMapVillage, FolderName + "VillageAssets.png");
}
void BackgroundDetailGen::shutdown()
{
    m_NoisePosVals = {};
}
} // namespace mmt_gd