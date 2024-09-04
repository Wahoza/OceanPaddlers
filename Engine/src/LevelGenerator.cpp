#include "stdafx.h"
#include "TileNames.hpp"
#include "EventBus.hpp"

#include "GameObjectEvents.hpp"

#include <random>

#include "LevelGenerator.hpp"
#include "TileNames.hpp"

namespace mmt_gd
{
void LevelGenerator::Initialize(const std::unique_ptr<tson::Map>& map, const fs::path& resourcePath, SpriteManager& spriteManager, int seed)
{
    _seed = seed;

    BackgroundGen.init(map, resourcePath, spriteManager);

    DetailGen.init({0, 0, BackgroundGen.GetTileSize().x, BackgroundGen.GetTileSize().y}, BackgroundGen.GetTotalSize(), seed);

    ObstacleGen.init({0, 0, BackgroundGen.GetTileSize().x, BackgroundGen.GetTileSize().y},
                     BackgroundGen.GetTotalSize(),
                     seed,
                     spriteManager.getWindow());

    TransObj.init(BackgroundGen.GetTotalSize(), 40);

    ObstacleGen.DisableGenArea(TransObj.getDisabledArea());

    BackgroundGen.RemoveUselessTiles();
    if (isInit)
    {
        return;
    }

    isInit = true;
    EventBus::getInstance().addListener(SwapTiles::Type, [this](IEvent::Ptr event) { SwapTileForAll(event); });
    EventBus::getInstance().addListener(SwapTileRequest::Type, [this](IEvent::Ptr event) { DoSwap(event); });
    EventBus::getInstance().addListener(IslandCollisionEvent::Type, [this](IEvent::Ptr event) { HandleIslandDestruction(event); });
}

void LevelGenerator::update(float deltaTime)
{
    BackgroundGen.update(deltaTime, m_lastWindowRect);
    ObstacleGen.update(deltaTime, m_lastWindowRect);
    DetailGen.update(deltaTime, m_lastWindowRect);
    TransObj.update(deltaTime, m_lastWindowRect);

    if (isTransitioning)
    {
        if (elapsedTransitionTime >= transitionTime)
        {
            BackgroundGen.LoadFromFolder(swapString);
            ObstacleGen.LoadTextures(swapString, {0, 0, 16, 16}, currentSwapID);
            DetailGen.LoadTextures(swapString);
            isTransitioning = false;
        }
        elapsedTransitionTime += deltaTime;
    }
}
void LevelGenerator::clearWindow(sf::RenderWindow& window)
{
    BackgroundGen.clearWindow(window);
}
void LevelGenerator::drawBackground(sf::RenderWindow& window)
{
    m_lastWindowRect = {window.getView().getCenter().x - window.getView().getSize().x / 2,
     window.getView().getCenter().y - window.getView().getSize().y / 2,
     window.getView().getSize().x,
     window.getView().getSize().y
    };

    ObstacleGen.drawWater(window);
    BackgroundGen.draw(window);
}

void LevelGenerator::drawForeground(sf::RenderWindow& window)
{
    ObstacleGen.draw(window);
    DetailGen.draw(window);
    TransObj.draw(window);
}
void LevelGenerator::shutdown()
{
    BackgroundGen.shutdown();
    DetailGen.shutdown();
    ObstacleGen.shutdown();
    TransObj.shutdown();
    
    isTransitioning = false;

}
void LevelGenerator::SwapTileForAll(IEvent::Ptr event)
{
    swapString = "grassTiles";
    if (auto swapEvent = std::dynamic_pointer_cast<SwapTiles>(event))
    {
        currentSwapID = std::get<0>(swapEvent.get()->getData());
        
        swapString = TileNames::getFolderLocation(currentSwapID);
        
        transitionTime = std::get<1>(swapEvent.get()->getData()) / 2.f;
        elapsedTransitionTime = 0;

        isTransitioning = true;
    }

    swapString += "/";
}
void LevelGenerator::DoSwap(IEvent::Ptr event)
{
    std::mt19937 gen(_seed + TransObj.getPosition().x);
    std::uniform_int_distribution<> dist(0, TileNames::getLevelCount()-1);

    std::tuple  eventData(dist(gen), 3);
    IEvent::Ptr swapEvent = std::make_shared<SwapTiles>(eventData);
    EventBus::getInstance().fireEvent(swapEvent);
}
void LevelGenerator::HandleIslandDestruction(IEvent::Ptr event)
{
    if (auto destroyEvent = std::dynamic_pointer_cast<IslandCollisionEvent>(event))
    {
        sf::FloatRect _shape = destroyEvent->getData();
        DetailGen.IslandDestructionListener(_shape);
        ObstacleGen.IslandDestroyListen(_shape);
    }
}
} // namespace mmt_gd