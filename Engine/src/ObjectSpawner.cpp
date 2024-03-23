#include "stdafx.h"
#include "ObjectSpawner.hpp"

#include "GameObject.hpp"
#include "CollectibleComponent.hpp"
#include "SpriteRenderComponent.hpp"

#include "SpriteManager.hpp"

#include "EventBus.hpp"
#include "GameObjectEvents.hpp"

namespace mmt_gd
{
void mmt_gd::ObjectSpawner::init(int TileSize, sf::RenderWindow* window)
{
    m_window    = window;
    _tileSize = TileSize;
}
void ObjectSpawner::createCollectible(sf::Vector2f pos)
{

    auto gameObject = GameObject::create("Collectible" + std::to_string(CollectibleCtr));
    gameObject->setPosition(pos + Vector2f{16, 16});

    gameObject->addComponent<CollectibleComponent>(*gameObject,
                                                   "pp",
                                                   static_cast<float>(16 * 2),
                                                   static_cast<float>(16 * 2));
        
    const auto renderComp = gameObject->addComponent<SpriteRenderComponent>(*gameObject,
                                                                            *m_window, "../assets/egg.png",
                                                                            "GameObjects");
    renderComp->init();
    renderComp->getSprite().setTextureRect({0, 0, 32, 32});
    renderComp->getSprite().setScale({1,1});
    renderComp->ShowSprite();
    EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(gameObject));

    CollectibleCtr++;
}
void ObjectSpawner::createFish(sf::Vector2f pos)
{
    FishCtr++;
}
} // namespace mmt_gd