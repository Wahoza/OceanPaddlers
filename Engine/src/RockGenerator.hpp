#pragma once
#include "stdafx.h"
#include "StaticColliderComponent.hpp"
#include "TriggerComponent.hpp"

#pragma once
namespace mmt_gd
{
class RockGenerator
{
public:
    void init(sf::Vector2i Area, int seed);
    void generateNextTile(float xpos, int pathy);
    void draw(sf::RenderWindow& window);
    void shutdown();

private:
    struct Obstacle
    {
        StaticColliderComponent* collider;
        sf::Vector2f             pos;
        sf::Vector2i             LeftTop;
    };

    std::vector<std::vector<Obstacle>>                Rocks;
    sf::Texture                                       m_texture;
    sf::Sprite                                        m_Sprite;
    int                                               TextureSize = 16;

    Vector2i _Area;
    int      wallsize = 4;
    int      chance   = 30;
    int      _seed;

    struct Current
    {
        TriggerComponent* trigger;
        sf::Vector2f      pos;
        sf::Vector2i      LeftTop;
    };
    std::vector<std::vector<Current>> Currents;
    IntRect                         currentArea    = {0, 0, 0, 0};
    int       currentChance = 30;
    Vector2i  minCurrentSize = {2, 2};
    Vector2i  maxCurrentSize = {3, 4};
    int maxOffset      = 3;
};
}