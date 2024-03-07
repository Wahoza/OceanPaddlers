#pragma once
#include "stdafx.h"
#include "EndlessBackgroundGenerator.hpp"
namespace mmt_gd
{
class LevelGenerator
{
public:
    void Initialize(const std::unique_ptr<tson::Map>& map, const fs::path& resourcePath, SpriteManager& spriteManager);
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    void shutdown();

private:
    EndlessBackgroundGenerator BackgroundGen;
};
}