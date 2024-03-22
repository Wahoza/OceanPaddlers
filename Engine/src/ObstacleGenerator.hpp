#include "stdafx.h"
#include "FastNoiseLite.h"

#include "StaticColliderComponent.hpp"

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
        bool endTile = false; //for Rocks and docks,...
        bool     print   = false;
        float    noise   = 0;
        Vector2i LeftTop = {0, 0};
        Vector2f pos     = {0, 0};  
    };

    void init(sf::IntRect _TextureRect, sf::Vector2i Area, int seed);
    void update(float deltaTime, sf::FloatRect windowRect);
    void draw(sf::RenderWindow& window);

    void shutdown();

private:
    void generateNextSprite();

    FastNoiseLite noise;
    int           waitingDist = 0; //purpose: Have a bit of a prefab to ease people into game
    bool          waiting     = true;

    std::vector<std::vector<StaticColliderComponent>> Colliders;
    std::vector<std::vector<NoisePositionValue>> NPVs;
    std::vector<std::vector<ObstacleTiles>>             ObstacleSpriteMap;

    Sprite                                            m_printer;
    Texture                                      m_texture;
    float                                        threshold = 0.2;
    float                                        scaleVal = 30;
    int                                          TileLength = 0;
};
}