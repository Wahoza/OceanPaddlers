#include "stdafx.h"
#include "PathGenerator.hpp"
#include <random>

namespace mmt_gd
{
void PathGenerator::init(int seed, sf::Vector2f startPos, sf::Vector2i area)
{
    m_seed = seed;
    VecStart = startPos;
    _area    = area;
    currentVec = {1, 0};
    m_length = 3;
}
std::vector<float> PathGenerator::GetPath(int xpos)
{
    int x = CalculatePathAtPos(xpos);
    std::vector<float> tempVec = {};
    for (int i = 0; i < _area.y; i++)
    {
        if (i <= x - pathR || i >= x + pathR)
        {
            tempVec.push_back(0);
        }
        else
        {
            tempVec.push_back(-2);
        }
    }
    return tempVec;
}
int PathGenerator::CalculatePathAtPos(int xpos)
{
    if ((m_length + VecStart.x) < xpos){
        VecStart += currentVec * static_cast<float>(m_length);

    static std::mt19937                    gen(m_seed + xpos);
    static std::uniform_int_distribution<> direction(-_area.y + wallSize, _area.y - wallSize);
    static std::uniform_int_distribution<> length(3, 7);

    int directionResult = direction(gen);

    // Adjust directionResult to ensure it doesn't overlap with the walls
    int upperBound = _area.y - wallSize - pathR;
    int lowerBound = wallSize + pathR;
    if (directionResult + VecStart.y < lowerBound)
    {
        directionResult = lowerBound - VecStart.y;
    }
    else if (directionResult + VecStart.y > upperBound)
    {
        directionResult = upperBound - VecStart.y;
    }

    m_length = length(gen);
    float m  = std::sqrt(static_cast<float>(m_length * m_length + directionResult * directionResult));

    currentVec = {m_length / m, directionResult / m};
}

    return currentVec.y * (xpos-VecStart.x)+VecStart.y;
}

void PathGenerator::shutdown()
{
    m_seed     = 0;
    pathR      = 3;
    currentVec = {0, 0};
    VecStart   = {0, 0};
    _area      = {0, 0};
    wallSize   = 3;
    m_length   = 0;
}

} // namespace mmt_gd