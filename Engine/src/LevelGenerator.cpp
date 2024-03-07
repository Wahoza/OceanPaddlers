#include "stdafx.h"
#include "LevelGenerator.hpp"
namespace mmt_gd
{
void LevelGenerator::Initialize(const std::unique_ptr<tson::Map>& map, const fs::path& resourcePath, SpriteManager& spriteManager)
{
    BackgroundGen.loadTileLayers(map, resourcePath, spriteManager);
}
void LevelGenerator::update(float deltaTime)
{
    BackgroundGen.update(deltaTime);
}
void LevelGenerator::draw(sf::RenderWindow& window)
{
    BackgroundGen.draw(window);
}
void LevelGenerator::shutdown()
{
    BackgroundGen.shutdown();
}
} // namespace mmt_gd