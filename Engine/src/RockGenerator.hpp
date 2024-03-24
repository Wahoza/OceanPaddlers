#include "stdafx.h"
#include "StaticColliderComponent.hpp"
#pragma once
namespace mmt_gd
{
class RockGenerator
{
public:
    void init(sf::Vector2i Area, int seed);
    void generateNextTile(float xpos);
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
    int      wallsize = 3;
    int      chance   = 3;
    int      _seed;
};
}