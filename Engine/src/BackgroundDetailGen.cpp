#include "stdafx.h"
#include "BackGroundDetailGen.hpp"
namespace mmt_gd
{
void mmt_gd::BackgroundDetailGen::init(sf::Texture texture, sf::IntRect _TextureRect, sf::Vector2i Area)
{
    noise.SetSeed(seed);
    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    m_texture = texture;
    for (int i = 0; i < texture.getSize().x / 16; i++)
    {
        std::shared_ptr<sf::Sprite> t_sprite = std::make_shared<sf::Sprite>();

        t_sprite->setTexture(m_texture);
        t_sprite->setTextureRect({i * _TextureRect.width, 0, _TextureRect.width, _TextureRect.height});
        m_SpriteMap[i] = t_sprite;

        NoiseToMapSprite += 1;
    }
    //generation stuff here
    for (int x = Area.x; x >= 0 ; x--)
    {
        std::vector<NoisePositionValue> SubVector;
        for (int y = Area.y; y >= 0; y--)
        {
            NoisePositionValue NPV;
            NPV.noise                   = noise.GetNoise(float(x * scaleVal), float(y * scaleVal));
            
            NPV.pos   = {float(x * 16), float(y * 16)};

            SubVector.push_back(NPV);
            /*std::cout << "noise: " + std::to_string(NPV.noise) + " pos: " + std::to_string(NPV.pos.x) + " " +
                             std::to_string(NPV.pos.y)
                      << std::endl;*/
        }
        m_NoisePosVals.push_back(SubVector);
    }
    
    NoiseToMapSprite = NoiseToMapSprite / 2;
    TileLength       = m_SpriteMap[0]->getTextureRect().width;
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

            //note: this is for testing, will be changed later with adjusted values for each structure type
            m_SpriteMap[int((NPV.noise+1)*NoiseToMapSprite)]->setPosition(NPV.pos);
            window.draw(*m_SpriteMap[int((NPV.noise+1)*NoiseToMapSprite)]);
        }
    }
}
} // namespace mmt_gd