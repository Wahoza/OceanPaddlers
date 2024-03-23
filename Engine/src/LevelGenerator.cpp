#include "stdafx.h"
#include <random>

#include "LevelGenerator.hpp"

namespace mmt_gd
{
void LevelGenerator::Initialize(const std::unique_ptr<tson::Map>& map, const fs::path& resourcePath, SpriteManager& spriteManager)
{
    std::random_device rd;
    std::mt19937       generator(rd());

    std::uniform_int_distribution<int> distribution(0, 1000);
    int                                seed = distribution(generator);

    BackgroundGen.init(map, resourcePath, spriteManager);

    DetailGen.init({0, 0, BackgroundGen.GetTileSize().x, BackgroundGen.GetTileSize().y}, BackgroundGen.GetTotalSize(), seed);

    ObstacleGen.init({0, 0, BackgroundGen.GetTileSize().x, BackgroundGen.GetTileSize().y}, BackgroundGen.GetTotalSize(), seed, spriteManager.getWindow());
}

void LevelGenerator::update(float deltaTime)
{
    BackgroundGen.update(deltaTime, m_lastWindowRect);
    ObstacleGen.update(deltaTime, m_lastWindowRect);
    DetailGen.update(deltaTime, m_lastWindowRect);
}

void LevelGenerator::draw(sf::RenderWindow& window)
{
    m_lastWindowRect = {window.getView().getCenter().x - window.getView().getSize().x / 2,
     window.getView().getCenter().y - window.getView().getSize().y / 2,
     window.getView().getSize().x,
     window.getView().getSize().y
    };

    BackgroundGen.draw(window);
    ObstacleGen.draw(window);
    DetailGen.draw(window);
}
void LevelGenerator::shutdown()
{
    BackgroundGen.shutdown();
    DetailGen.shutdown();
    ObstacleGen.shutdown();
}
} // namespace mmt_gd