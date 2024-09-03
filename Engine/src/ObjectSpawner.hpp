#pragma once
#include "stdafx.h"
namespace mmt_gd
{
class ObjectSpawner
{
public:
    void init(int TileSize, sf::RenderWindow* window, int seed);
    void createPotentialCollectible(sf::Vector2f pos);
    void createFish(sf::Vector2f pos);

private:
    int               waitVal = 30;
    int               CallsSinceLastSpawn = 0;
    int               minCalls            = 10;
    int               maxCalls            = 35;
    int               _seed = 0;

    int _tileSize;
    int CollectibleCtr = 0;
    int FishCtr = 0;


    sf::RenderWindow* m_window;
    std::string            Powerup = "powerupCollectible.png";
    std::string       Heart   = "heartCollectible.png";
    std::string            Egg     = "eggCollectible.png";
    std::string            Path    = "../assets/collectibles/";
};
}