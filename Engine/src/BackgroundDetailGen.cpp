#include "stdafx.h"
#include "BackGroundDetailGen.hpp"
namespace mmt_gd
{
void mmt_gd::BackgroundDetailGen::init(sf::IntRect _TextureRect, sf::Vector2i Area)
{
    if (!m_texture.loadFromFile("../assets/noiseAssets.png"))
    {
        sf::err() << "Could not load icon\n";
        return;
    }

    noise.SetSeed(seed);
    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);

    noiseLocal.SetSeed(seed);
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

            SubVector.push_back(NPV);
            /*std::cout << "noise: " + std::to_string(NPV.noise) + " pos: " + std::to_string(NPV.pos.x) + " " +
                             std::to_string(NPV.pos.y)
                      << std::endl;*/
        }
        m_NoisePosVals.push_back(SubVector);
    }
    
    TileLength       = m_SpriteMap[0]->getTextureRect().width;

    initSpriteMap(m_SpriteMapField, "FieldAssets.png");
    initSpriteMap(m_SpriteMapForrest, "ForrestAssets.png");
    initSpriteMap(m_SpriteMapMines, "MineAssets.png");
    initSpriteMap(m_SpriteMapVillage, "VillageAssets.png");
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
            NPV.pos.x += TileLength;
            NPV.noise = noise.GetNoise(NPV.pos.x / TileLength * scaleVal, NPV.pos.y / TileLength * scaleVal);
            NPV.localNoise = noiseLocal.GetNoise(NPV.pos.x / TileLength * scaleVal * 20, NPV.pos.y / TileLength * scaleVal * 20);
        }

        m_NoisePosVals.insert(m_NoisePosVals.begin(), SubVector);
    }
}
void BackgroundDetailGen::draw(sf::RenderWindow& window)
{
    for (auto& obj : m_NoisePosVals)
    {
        for (NoisePositionValue& NPV : obj)
        {
            if (NPV.pos.y > 2 * 16 && NPV.pos.y < 14 * 16)
                continue;

            ////note: this is for testing, will be changed later with adjusted values for each structure type
            //m_SpriteMap[int((NPV.noise+1)*NoiseToMapSprite)]->setPosition(NPV.pos);
            //window.draw(*m_SpriteMap[int((NPV.noise+1)*NoiseToMapSprite)]);

            //note: No switch statement here due to floating point values
            sf::Sprite t_sprite;
            if (NPV.noise < -0.3)
            {
                t_sprite = *m_SpriteMapForrest[NoiseToInt(m_SpriteMapForrest.size(), float(NPV.localNoise))];
            }
            else if (NPV.noise < -0.1)
            {
                t_sprite = *m_SpriteMapField[NoiseToInt(m_SpriteMapField.size(), float(NPV.localNoise))];
            }
            else if (NPV.noise < 0.1)
            {
                t_sprite = *m_SpriteMapVillage[NoiseToInt(m_SpriteMapVillage.size(), float(NPV.localNoise))];
            }
            else if (NPV.noise < 0.7)
            {
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
} // namespace mmt_gd