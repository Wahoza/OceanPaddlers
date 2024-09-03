#include "stdafx.h"
#include "ObjectSpawner.hpp"

#include "GameObject.hpp"
#include "CollectibleComponent.hpp"
#include "SpriteRenderComponent.hpp"

#include "SpriteManager.hpp"

#include "EventBus.hpp"
#include "GameObjectEvents.hpp"

#include "AIController.hpp"
#include "RigidBodyComponent.hpp"

#include <random>

namespace mmt_gd
{
void mmt_gd::ObjectSpawner::init(int TileSize, sf::RenderWindow* window, int seed)
{
    m_window    = window;
    _tileSize = TileSize;
    _seed       = seed;
}
void ObjectSpawner::createPotentialCollectible(sf::Vector2f pos)
{
    if (pos.x / 16 < waitVal)
    {
        return;
    }

    CallsSinceLastSpawn++;

    if (CallsSinceLastSpawn < minCalls)
    {
        return;
    }

    static std::mt19937                    genCollectible(_seed + pos.x + pos.y);
    static std::uniform_int_distribution<> collectibleSpread(0, 400);

    int collectibleResult = collectibleSpread(genCollectible);

    if (!(collectibleResult <= 20 || CallsSinceLastSpawn >= maxCalls))
    {
        return;
    }
    else if (CallsSinceLastSpawn >= maxCalls)
    {
        collectibleResult = 10;
    }


    auto gameObject = GameObject::create("Collectible" + std::to_string(CollectibleCtr));
    gameObject->setPosition(pos + Vector2f{8, 8});


    string type = "Egg";
    string textureLocation = Path + Egg;
    //std::cout << collectibleResult << std::endl;
    if (collectibleResult >= 15)
    {
        type = "Health";
        textureLocation = Path + Heart;
    }
    if (collectibleResult >= 19)
    {
        type            = "PowerUp";
        textureLocation = Path + Powerup;
    }
    gameObject->addComponent<CollectibleComponent>(*gameObject,
                                                   type,
                                                   static_cast<float>(16),
                                                   static_cast<float>(16));
        
    const auto renderComp = gameObject->addComponent<SpriteRenderComponent>(*gameObject,
                                                                            *m_window,
                                                                            textureLocation,
                                                                            "GameObjects");
    renderComp->init();
    renderComp->getSprite().setTextureRect({0, 0, 16, 16});
    renderComp->getSprite().setScale({1,1});
    renderComp->ShowSprite();
    EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(gameObject));

    CollectibleCtr++;
    CallsSinceLastSpawn = 0;
}
void ObjectSpawner::createFish(sf::Vector2f pos)
{
    if (pos.x / 16 < waitVal)
    {
        return;
    }

    auto gameObject = GameObject::create("Fish" + std::to_string(FishCtr));
    gameObject->setPosition(pos);

    gameObject->addComponent<AIController>(*gameObject);

    //FloatRect shape, Vector2f offset, string forces, float mass, float Loss
    gameObject->addComponent<RigidBodyComponent>(*gameObject, FloatRect{0, 0, 16, 16}, Vector2f(0, 0), "", 3.0, 0.2);

    const auto renderComp = gameObject->addComponent<SpriteRenderComponent>(*gameObject,
                                                                            *m_window,
                                                                            "../assets/pushfish.png",
                                                                            "Background");
    renderComp->init();
    renderComp->getSprite().setTextureRect({0, 0, 32, 32});
    renderComp->getSprite().setScale({0.5, 0.5});
    renderComp->ShowSprite();
    EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(gameObject));

    FishCtr++;
}
} // namespace mmt_gd