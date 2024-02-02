#include "stdafx.h"

#include "ObjectFactory.hpp"

#include "GameObjectEvents.hpp"
#include "PlayerMoveComponent.hpp"
#include "SpriteRenderComponent.hpp"
#include "Tileson.hpp"
#include "RigidBodyComponent.hpp"
#include "StaticColliderComponent.hpp"
#include "TetherComponent.hpp"
#include "PhysicsManager.hpp"
#include "Hud.hpp"
#include "AIController.hpp"
#include "ScoreConverter.hpp"

namespace mmt_gd
{
static GameObject::Ptr loadSprite(tson::Object&        object,
                                  const std::string&   layer,
                                  const fs::path&      resourcePath,
                                  const SpriteManager& spriteManager)
{
    auto gameObject = GameObject::create(object.getName());

    gameObject->setPosition(static_cast<float>(object.getPosition().x), static_cast<float>(object.getPosition().y));

    // Parse data from file
    sf::IntRect textureRect{};
    textureRect.width  = object.getSize().x;
    textureRect.height = object.getSize().y;
    fs::path spriteTexture;
    auto     input     = false;
    auto     playerIdx = 0;
    

    for (const auto* property : object.getProperties().get())
    {
        if (auto name = property->getName(); name == "Texture")
        {
            spriteTexture = resourcePath / std::any_cast<std::string>(property->getValue());
        }
        else if (name == "TextureRectLeft")
        {
            textureRect.left = std::any_cast<int>(property->getValue());
        }
        else if (name == "TextureRectTop")
        {
            textureRect.top = std::any_cast<int>(property->getValue());
        }
        else if (name == "TextureRectWidth")
        {
            textureRect.width = std::any_cast<int>(property->getValue());
        }
        else if (name == "TextureRectHeight")
        {
            //std::cout << std::any_cast<string> (property->getValue());
            textureRect.height = std::any_cast<int>(property->getValue());
        }
        else if (name == "InputPlayerIdx")
        {
            playerIdx = std::any_cast<int>(property->getValue());
        }
        else if (name == "Input")
        {
            input = std::any_cast<bool>(property->getValue());
        }
        else if (name == "Mass")
        {

            auto mass = std::any_cast<float>(property->getValue());
            string forces;
            if (object.getProperties().getProperty("Forces"))
            {
                forces = std::any_cast<string>(object.getProperties().getProperty("Forces")->getValue());
            }
            else
            {
                forces = " ";
            }
            float loss;
            if (object.getProperties().getProperty("VelocityLoss"))
            {
                loss = std::any_cast<float>(object.getProperties().getProperty("VelocityLoss")->getValue());
            }
            else
            {
                loss = 0;
            }

            if (object.getProperties().getProperty("TetherPercentage"))
            {
                float percentage = std::any_cast<float>(object.getProperties().getProperty("TetherPercentage")->getValue());
                gameObject->addComponent<TetherComponent>(*gameObject,
                                                             FloatRect{0,
                                                                       0,
                                                                       static_cast<float>(object.getSize().x),
                                                                       static_cast<float>(object.getSize().y)},
                                                             Vector2f{0.f, 0.f},
                                                             forces,
                                                             mass,
                                                             loss,
                                                             true,
                                                             percentage
                    );
            }
            else
            {
                gameObject->addComponent<RigidBodyComponent>(*gameObject,
                                                             FloatRect{0,
                                                                       0,
                                                                       static_cast<float>(object.getSize().x),
                                                                       static_cast<float>(object.getSize().y)},
                                                             Vector2f{0.f, 0.f},
                                                             forces,
                                                             mass,
                                                             loss);
            }

        }
        else if (name == "Collectible")
        {
            gameObject->addComponent<CollectibleComponent>(*gameObject,
                                                           "pp",
                                                           static_cast<float>(object.getSize().x),
                                                           static_cast<float>(object.getSize().y));
        }
    }

    if (spriteTexture.string().length() > 0)
    {
        const auto renderComp = gameObject->addComponent<SpriteRenderComponent>(*gameObject,
                                                                                spriteManager.getWindow(),
                                                                                spriteTexture.string(),
                                                                                layer);

        renderComp->getSprite().setTextureRect(textureRect);
        float    scaleX = static_cast<float>(object.getSize().x) / static_cast<float>(textureRect.width);
        float    scaleY = static_cast<float>(object.getSize().y) / static_cast<float>(textureRect.height);
        Vector2f scale  = {scaleX, scaleY};
        renderComp->getSprite().setScale(scale);
    }


    if (input)
    {
        if (playerIdx < 2)
        {
            gameObject->addComponent<PlayerMoveComponent>(*gameObject, playerIdx, true);
        }
        else
        {
            gameObject->addComponent<AIController>(*gameObject);
        }

    }

    if (!gameObject->init())
    {
        sf::err() << "Could not initialize go " << gameObject->getId() << " in TileMap " << std::endl;
    }

    EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(gameObject));

    return gameObject;
}


GameObject::Ptr ObjectFactory::processTsonObject(tson::Object&        object,
                                                 const tson::Layer&   layer,
                                                 const fs::path&      path,
                                                 const SpriteManager& spriteManager)
{
    if (object.getType() == "Sprite")
    {
        auto sprite = loadSprite(object, layer.getName(), path, spriteManager);
    }
    if (object.getType() == "Collider")
    {
        auto property =object.getProperties().get();
        
        if (property[0]->getName() == "CollisionType")
        {
            auto type = std::any_cast<string>(property[0]->getValue());

            StaticColliderComponent collider = *new StaticColliderComponent({static_cast<float>(object.getPosition().x),
                                                                             static_cast<float>(object.getPosition().y),
                                                                             static_cast<float>(object.getSize().x),
                                                                             static_cast<float>(object.getSize().y)},
                                                                            type);
        }

        
    }
    if (object.getType() == "Trigger")
    {
        string identifier = "";
        string notification = "";
        for (const auto* property : object.getProperties().get())
        {
            if (property->getName() == "Notification")
            {
                notification = std::any_cast<string>(property->getValue());
            }
            else if (property->getName() == "Identifier")
            {
                identifier = std::any_cast<string>(property->getValue());
            }
        }
        //auto trigger = loadTrigger(object, layer.getName());
        TriggerComponent collider = *new TriggerComponent({static_cast<float>(object.getPosition().x),
                                                                         static_cast<float>(object.getPosition().y),
                                                                         static_cast<float>(object.getSize().x),
                                                                         static_cast<float>(object.getSize().y)},
                                                                        identifier, notification);
    }
    if (object.getType() == "ScoreConverter")
    {
        string path = "";
        int    amount = 0;
        for (const auto* property : object.getProperties().get())
        {
            if (property->getName() == "FilePath")
            {
                path = std::any_cast<string>(property->getValue());
            }
            if (property->getName() == "DisplayedScores")
            {
                amount = std::any_cast<int>(property->getValue());
            }
        }
        auto scoregameObject     = GameObject::create(object.getName());
        scoregameObject->setPosition(static_cast<float>(object.getPosition().x), static_cast<float>(object.getPosition().y));

        scoregameObject->addComponent<ScoreConverter>(*scoregameObject, spriteManager.getWindow(), path, amount);
        EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(scoregameObject));
    }

    return {};
}
} // namespace mmt_gd
