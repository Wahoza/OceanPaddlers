#pragma once
#include "stdafx.h"
namespace mmt_gd
{
class PathGenerator
{
public:
    void               init(int seed, sf::Vector2f startPos, sf::Vector2i area);
    std::vector<float> GetPath(int xpos);
    int                CalculatePathAtPos(int xpos);
    void               shutdown();

private:
    int m_seed = 0;
    int          pathR      = 3;
    float        maxVal      = -3;
    sf::Vector2f currentVec = {0, 0};
    sf::Vector2f VecStart   = {0, 0};
    sf::Vector2i _area       = {0,0};
    int          wallSize   = 2;
    int          m_length    = 0;
};
}