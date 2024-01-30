#include "stdafx.h"

#include "MainState.hpp"

#include "CameraRenderComponent.hpp"
//#include "Hud.hpp"
#include "PlayerMoveComponent.hpp"
#include "Game.hpp"
#include "InputManager.hpp"
#include "TileMapLoader.hpp"
#include "Tileson.hpp"
#include "TransformAnimationComponent.hpp"
#include "TransformAnimationSmoothFollow.hpp"
#include "AIController.hpp"
#include <memory>
#include <thread>

namespace mmt_gd
{
MainState::MainState(GameStateManager* gameStateManager, Game* game) :
GameState(gameStateManager, game),
m_spriteManager(game->getWindow())
{
}

void MainState::init()
{
    PROFILE_FUNCTION();

    m_gameObjectManager.init();
    m_spriteManager.init();
    m_PhysicsManager = PhysicsManager::getInstance();
    m_Hud            = Hud::getInstance();

    // load tile map/level
    {
        PROFILE_SCOPE("Load Tile Map");
        const auto resourcePath = fs::path{"../"} / "assets";
        const auto mapPath      = resourcePath / "game.tmj";

        tson::Tileson t;
        if (const std::unique_ptr<tson::Map> map = t.parse(mapPath); map->getStatus() == tson::ParseStatus::OK)
        {
            TileMapLoader::loadTileLayers(map, resourcePath, m_spriteManager);
            TileMapLoader::loadObjectLayers(map, resourcePath, m_spriteManager);
        }
        else
        {
            map->getStatus();
            std::cerr << "Error loading " << mapPath << std::endl;
        }
    }

    const auto player = m_gameObjectManager.getGameObject("Player0");
   
    // Moving 

    {
        float scaleFactor = m_game->getWindow().getSize().y / ((m_spriteManager.getTileSize().y-2) * 16);
        sf::View gameView = View(FloatRect(0, 0, m_game->getWindow().getSize().x / scaleFactor, m_game->getWindow().getSize().y / scaleFactor));
        //gameView.zoom(1.5);
        m_game->getWindow().setView(gameView);

        const auto camera = GameObject::create("Camera");
        camera->setPosition(sf::Vector2f((m_game->getWindow().getSize().x / 2), (m_game->getWindow().getSize().y / 2)));
        const auto renderComp = camera->addComponent<CameraRenderComponent>(*camera,
                                                                            m_game->getWindow(),
                                                                            m_game->getWindow().getView());
        
        camera->addComponent<TransformAnimationComponent>(*camera,
                                                          std::make_shared<mmt::TransformAnimationSmoothFollow>(player, 10.F));


        if (!camera->init())
            ffErrorMsg("Could not initialize camera");

        m_gameObjectManager.addGameObject(camera);
        m_spriteManager.setCamera(renderComp.get());        
    }
 


    // Define layer order manually here. Could come from custom file settings.
    m_spriteManager.setLayerOrder({"Floor", "Background", "Objects", "GameObjects", "Top"});
}

void MainState::update(const float deltaTime)
{
    PROFILE_FUNCTION();
    if (InputManager::getInstance().isKeyPressed("Exit"))
    {
        m_gameStateManager->setState("MenuState");
        return;
    }

    EventBus::getInstance().processEvents(deltaTime);
    m_gameObjectManager.update(deltaTime);

    //m_PhysicsManager = PhysicsManager::getInstance();
    m_PhysicsManager->HandleCollisions(deltaTime);
    m_Hud->update(deltaTime);
}

void MainState::draw()
{
    PROFILE_FUNCTION();
    m_spriteManager.draw();
    m_Hud->draw(m_game->getWindow());
}

void MainState::exit()
{
    PROFILE_FUNCTION();
    m_spriteManager.shutdown();
    m_gameObjectManager.shutdown();
    m_PhysicsManager->shutdown();
    m_Hud->shutdown();
}
} // namespace mmt_gd
