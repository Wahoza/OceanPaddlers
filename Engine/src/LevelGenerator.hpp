#pragma once
#include "stdafx.h"
#include "EndlessBackgroundGenerator.hpp"
#include "BackGroundDetailGen.hpp"
#include "ObstacleGenerator.hpp"

namespace mmt_gd
{
class LevelGenerator
{
public:
    void Initialize(const std::unique_ptr<tson::Map>& map, const fs::path& resourcePath, SpriteManager& spriteManager);
    void update(float deltaTime);
    void drawBackground(sf::RenderWindow& window);
    void drawForeground(sf::RenderWindow& window);
    void shutdown();

private:
    EndlessBackgroundGenerator BackgroundGen;
    ObstacleGenerator          ObstacleGen;
    BackgroundDetailGen        DetailGen;

    sf::FloatRect m_lastWindowRect;
};
}