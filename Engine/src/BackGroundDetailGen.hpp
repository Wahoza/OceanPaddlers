#pragma once
#include "stdafx.h"
#include "IEvent.hpp"
namespace mmt_gd
{
class BackgroundDetailGen
{
public:
    struct NoisePositionValue
    {
        float    noise = -1;
        float        localNoise = -1;
        sf::Vector2f pos  = {0, 0};
        bool         draw = false;
    };
    void initSpriteMap(std::unordered_map<int, std::shared_ptr<sf::Sprite>>& map, std::string FileName);
    void init(sf::IntRect _TextureRect, sf::Vector2i Area, int m_seed);
    void update(float deltaTime, sf::FloatRect 
    );
    void IslandGenListener(IEvent::Ptr event);
    void draw(sf::RenderWindow& window);

    int NoiseToInt(int TotalSize, float noiseVal);

    void LoadTextures(std::string FolderName);
    void shutdown();

private:
    int m_seed = 420;

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

    std::vector<sf::Vector2f> m_IslandTiles;
    bool                      isInit = false;
};
} // namespace mmt_gd