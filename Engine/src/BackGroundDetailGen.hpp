#pragma once
#include "stdafx.h"
namespace mmt_gd
{
class BackgroundDetailGen
{
public:
    struct NoisePositionValue
    {
        float    noise;
        sf::Vector2f pos;
    };

    void init(sf::Texture texture, sf::IntRect _TextureRect, sf::Vector2i Area);
    void update(float deltaTime, sf::FloatRect windowRect);
    void draw(sf::RenderWindow& window);

private:
    int seed = 420;

    FastNoiseLite noise;
    std::unordered_map<int, std::shared_ptr<sf::Sprite>> m_SpriteMap;
    //idea: sprites as printers, noise position vector<vector<tuple>> like endless background. maybe different noise for houses. also: Random amount of trees / block
    sf::Texture                                           m_texture;
    std::vector<std::vector<NoisePositionValue>> m_NoisePosVals;
    
    float scaleVal         = 4;
    float TileLength       = 0;
    float NoiseToMapSprite = 0;
};
} // namespace mmt_gd