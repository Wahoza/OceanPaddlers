#pragma once
#include "stdafx.h"
#include "FastNoiseLite.h"

#include "PathGenerator.hpp"
#include "StaticColliderComponent.hpp"
#include "ObjectSpawner.hpp"

#include "StaticColliderComponent.hpp"
#include "RockGenerator.hpp"

#include <random>
namespace mmt_gd
{
class ObstacleGenerator
{
public:
    struct NoisePositionValue
    {
        float        noise = 0;
        float        localnoise = 0;
        sf::Vector2f pos = {0, 0};
    };
    struct ObstacleTiles
    {
        bool     print   = false;
        float    noise   = 0;
        Vector2i LeftTop = {0, 0};
        Vector2f pos     = {0, 0};  
        bool     isDirt  = false;
    };
    struct DockTiles
    {
        Vector2f pos;
        Vector2i LeftTop;
    };

    void init(sf::IntRect _TextureRect, sf::Vector2i Area, int seed, RenderWindow& window);
    void update(float deltaTime, sf::FloatRect windowRect);
    void draw(sf::RenderWindow& window);
    void drawWater(sf::RenderWindow& window);

    void shutdown();

private:

    void          generateNextSprite();
    void          generateNextDock(int type, std::vector<Vector2i> normalVec, Vector2i offset, Vector2f pos);
    void          GenerateDirtArea(float xpos);

    FastNoiseLite noise;
    int           waitingDist = 0; //purpose: Have a bit of a prefab to ease people into game
    bool          waiting     = true;

    std::vector<std::vector<StaticColliderComponent>> Colliders;
    std::vector<std::vector<NoisePositionValue>> NPVs;
    std::vector<std::vector<ObstacleTiles>>             ObstacleSpriteMap;

    Sprite                                            m_printer;
    Texture                                           m_texture;

    
    Sprite  m_dirtprinter;
    Texture m_dirttexture;

    Sprite                                m_waterPrinter;
    Texture                               m_waterTexture;

    std::vector<DockTiles>               DockTileVector;
    std::vector<StaticColliderComponent*> DockColliders;

    Sprite  m_dockPrinter;
    Texture m_dockTexture;

    
    int                                          _seed = 0;
    float                                        threshold = 0.2f;
    float                                        scaleVal = 30;
    int                                          TileLength = 0;
    int                                          pathy;
    PathGenerator PathGen;
    ObjectSpawner ObjSpawner;
    RockGenerator RockGen;

    int  collectibleCount = 0;

    StaticColliderComponent* WallColliderTop;
    StaticColliderComponent* WallColliderBottom;

    int       dirtSpawnChance = 60;
    FloatRect DirtAreaBarrier = {0, 0, 0, 0};
    FloatRect DirtArea = {0, 0, 0, 0};
    FloatRect min_dirtArea = {0, 0,10, 10};
    FloatRect max_dirtArea = {0, 0, 20, 20};
};
}