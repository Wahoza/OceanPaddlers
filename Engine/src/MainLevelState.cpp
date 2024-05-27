//Author: Radomir Dinic, modified by Kilian Kolb & Ema Selmanovic
//Description: MainState for the Game. Note: Camera is not generated from TileMap.
//License: LICENSE.txt
#include "stdafx.h"

#include "AiManager.hpp"
#include "CameraRenderComponent.hpp"
#include "MainLevelState.hpp"
//#include "Hud.hpp"
#include "AIController.hpp"
#include "EventBus.hpp"
#include "Game.hpp"
#include "GameObjectEvents.hpp"
#include "IEvent.hpp"
#include "InputManager.hpp"
#include "InputObject.hpp"
#include "PlayerMoveComponent.hpp"
#include "SoundManager.hpp"
#include "TileMapLoader.hpp"
#include "Tileson.hpp"
#include "TransformAnimationComponent.hpp"
#include "TransformAnimationSmoothFollow.hpp"

#include <memory>
#include <thread>

namespace mmt_gd
{
MainLevelState::MainLevelState(GameStateManager* gameStateManager, Game* game) :
GameState(gameStateManager, game),
m_spriteManager(game->getWindow())
{
    EventBus::getInstance().addListener(GameOverEvent::Type, [this](IEvent::Ptr event) { ListenforEnd(event); });
}

void MainLevelState::init()
{
    PROFILE_FUNCTION();
    isActive = true;
    m_soundManager.init();
    m_gameObjectManager.init();
    m_spriteManager.init();
    m_PhysicsManager = PhysicsManager::getInstance();
    m_Hud            = Hud::getInstance();
    m_Hud->CreateHud();

    // load tile map/level
    {
        PROFILE_SCOPE("Load Tile Map");
        const auto    resourcePath = fs::path{"../"} / "assets";
        const auto    mapPath      = resourcePath / "game.tmj";
        tson::Tileson t;
        if (const std::unique_ptr<tson::Map> map = t.parse(mapPath); map->getStatus() == tson::ParseStatus::OK)
        {
            TileMapLoader::init(map, resourcePath, m_spriteManager);
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
        float    scaleFactor = m_game->getWindow().getSize().y / ((m_spriteManager.getTileSize().y - 2) * 16);
        sf::View gameView    = View(
            FloatRect(0, 0, m_game->getWindow().getSize().x / scaleFactor, m_game->getWindow().getSize().y / scaleFactor));
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
    m_soundManager.playMusic("Music");
}

void MainLevelState::update(const float deltaTime)
{
    PROFILE_FUNCTION();

    if (InputManager::getInstance().isKeyPressed("Exit"))
    {
        paused = !paused;
        //m_gameStateManager->setState("GameOverState");
        bool         eventData(paused);
        IEvent::Ptr _pauseEvent = std::make_shared<PauseEvent>(eventData);
        EventBus::getInstance().fireEvent(_pauseEvent);
    }
    
    EventBus::getInstance().processEvents(deltaTime);

    if (paused)
    {
        return;
    }

    m_gameObjectManager.update(deltaTime);

    //m_PhysicsManager = PhysicsManager::getInstance();
    m_PhysicsManager->HandleCollisions(deltaTime);
    m_Hud->update(deltaTime);
    AiManager::getInstance()->update();
}

void MainLevelState::draw()
{
    PROFILE_FUNCTION();
    m_spriteManager.drawAll();
    m_Hud->draw(m_game->getWindow());
}

void MainLevelState::ListenforEnd(IEvent::Ptr event)
{
    if (isActive)
    {
        m_gameStateManager->setState("GameOverLevelState");
        isActive = false;
    }
}

void MainLevelState::exit()
{
    PROFILE_FUNCTION();
    m_spriteManager.shutdown();
    m_gameObjectManager.shutdown();
    m_PhysicsManager->shutdown();
    //m_Hud->shutdown();
    AiManager::getInstance()->shutdown();
    InputObject::getInstance()->shutdown();
    GameState::exit();
    m_soundManager.stopMusic("Music");
}

} // namespace mmt_gd
