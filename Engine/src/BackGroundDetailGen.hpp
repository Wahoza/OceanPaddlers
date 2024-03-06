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
        float        localNoise;
        sf::Vector2f pos;
    };

    void init(sf::IntRect _TextureRect, sf::Vector2i Area);
    void update(float deltaTime, sf::FloatRect windowRect);
    void draw(sf::RenderWindow& window);

    void initSpriteMap(std::unordered_map<int, std::shared_ptr<sf::Sprite>>& map, std::string FileName);

    int NoiseToInt(int TotalSize, float noiseVal);

private:
    int seed = 420;

    FastNoiseLite noise;
    FastNoiseLite                                        noiseLocal;
    std::unordered_map<int, std::shared_ptr<sf::Sprite>> m_SpriteMap;
    std::unordered_map<int, std::shared_ptr<sf::Sprite>> m_SpriteMapVillage;
    std::unordered_map<int, std::shared_ptr<sf::Sprite>> m_SpriteMapMines;
    std::unordered_map<int, std::shared_ptr<sf::Sprite>> m_SpriteMapForrest;
    std::unordered_map<int, std::shared_ptr<sf::Sprite>> m_SpriteMapField;


    //idea: sprites as printers, noise position vector<vector<tuple>> like endless background. maybe different noise for houses. also: Random amount of trees / block
    sf::Texture                                           m_texture;
    std::vector<std::vector<NoisePositionValue>> m_NoisePosVals;
    std::vector<std::shared_ptr<sf::Texture>>              m_textures;
    float scaleVal         = 4;
    float TileLength       = 0;

    std::string FolderName = "SegmentedTiles/";
};
} // namespace mmt_gd