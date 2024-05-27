#pragma once
#include "stdafx.h"
#include "EndlessBackgroundGenerator.hpp"
#include "BackGroundDetailGen.hpp"
#include "ObstacleGenerator.hpp"
#include "TransitionObject.hpp"

namespace mmt_gd
{
class LevelGenerator
{
public:
    void Initialize(const std::unique_ptr<tson::Map>& map, const fs::path& resourcePath, SpriteManager& spriteManager, int seed);
    void update(float deltaTime);

    void drawBackground(sf::RenderWindow& window);
    void drawForeground(sf::RenderWindow& window);
    void shutdown();

    void SwapTileForAll(IEvent::Ptr event);
    void DoSwap(IEvent::Ptr event);

private:
    EndlessBackgroundGenerator BackgroundGen;
    ObstacleGenerator          ObstacleGen;
    BackgroundDetailGen        DetailGen;
    TransitionObject           TransObj;

    sf::FloatRect m_lastWindowRect;

    string swapString = "grassTiles";

    bool isTransitioning = false;
    float transitionTime = 0;
    float elapsedTransitionTime = 0;
    int _seed;
};
}