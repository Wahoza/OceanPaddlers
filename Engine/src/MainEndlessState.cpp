//Author: Radomir Dinic, modified by Kilian Kolb & Ema Selmanovic
//Description: MainState for the Game. Note: Camera is not generated from TileMap.
//License: LICENSE.txt
#include "stdafx.h"

#include "MainEndlessState.hpp"
#include "AiManager.hpp"
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

#include "IEvent.hpp"
#include "GameObjectEvents.hpp"
#include "EventBus.hpp"
#include "CollisionParticleHandler.hpp"

#include "InputObject.hpp"
#include "SoundManager.hpp"

#include <memory>
#include <thread>

namespace mmt_gd
{
MainEndlessState::MainEndlessState(GameStateManager* gameStateManager, Game* game) :
GameState(gameStateManager, game),
m_spriteManager(game->getWindow())
{
    EventBus::getInstance().addListener(GameOverEvent::Type, [this](IEvent::Ptr event) { ListenforEnd(event); });
    //EventBus::getInstance().addListener(SwapTiles::Type, [this](IEvent::Ptr event) { ListenforEnd(event); });
}

void MainEndlessState::init()
{
    PROFILE_FUNCTION();

    std::random_device rd;
    std::mt19937       generator(rd());

    std::uniform_int_distribution<int> distribution(0, 1000);
    int                                seed = distribution(generator);

    isActive = true;
    m_soundManager.init();
    m_gameObjectManager.init();
    m_spriteManager.init();
    m_PhysicsManager = PhysicsManager::getInstance();

    ParticleManager::getInstance()->setGravity({0, 50});
    m_Hud.CreateHud();
    m_Hud.setSeed(seed);

    // load tile map/level
    {
        PROFILE_SCOPE("Load Tile Map");
        const auto resourcePath = fs::path{"../"} / "assets";
        
        std::string path = "gameEndless.tmj";
        if (!isEndless)
        {
            path = "game.tmj";
        }
        const auto mapPath      = resourcePath / path;

        tson::Tileson t;
        if (const std::unique_ptr<tson::Map> map = t.parse(mapPath); map->getStatus() == tson::ParseStatus::OK)
        {
            TileMapLoader::init(map, resourcePath, m_spriteManager);
            TileMapLoader::loadObjectLayers(map, resourcePath, m_spriteManager);

            if (isEndless)
            {
                m_gen.Initialize(map, resourcePath, m_spriteManager, seed);
            }
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

    m_soundManager.playMusic("Music");

    sf::FloatRect viewport(0.f, 0.f, 400.f, 300.f);

    isInit = true;
}

void MainEndlessState::update(const float deltaTime)
{
    PROFILE_FUNCTION();

    if (InputManager::getInstance().isKeyPressed("Exit"))
    {
        paused = !paused;
        //m_gameStateManager->setState("GameOverEndlessState");

        bool        eventData(paused);
        IEvent::Ptr _pauseEvent = std::make_shared<PauseEvent>(eventData);
        EventBus::getInstance().fireEvent(_pauseEvent);
    }

    EventBus::getInstance().processEvents(deltaTime);
    if (paused)
    {
        return;
    }
    m_gameObjectManager.update(deltaTime);
    if (isEndless)
    {
        m_gen.update(deltaTime);
    }

    //m_PhysicsManager = PhysicsManager::getInstance();
    m_PhysicsManager->HandleCollisions(deltaTime);
    m_Hud.update(deltaTime);
    AiManager::getInstance()->update();
    ParticleManager::getInstance()->update(deltaTime);
}

void MainEndlessState::draw()
{
    PROFILE_FUNCTION();
    if (isEndless)
    {
        m_gen.drawBackground(m_game->getWindow());
        m_spriteManager.drawBackground();
        m_spriteManager.drawCenter();
        m_gen.drawForeground(m_game->getWindow());
        m_spriteManager.drawTop();
    }
    else
    {
        m_spriteManager.drawAll();
    }
    ParticleManager::getInstance()->draw(m_game->getWindow());
    m_Hud.draw(m_game->getWindow());
}

void MainEndlessState::ListenforEnd(IEvent::Ptr event)
{
    if (isActive)
    {
        isActive = false;
        m_gameStateManager->setState("GameOverEndlessState");
    }
}

void MainEndlessState::exit()
{
    PROFILE_FUNCTION();
    m_spriteManager.shutdown();
    m_gameObjectManager.shutdown();
    m_PhysicsManager->shutdown();
    AiManager::getInstance()->shutdown();
    InputObject::getInstance()->shutdown();
    m_soundManager.stopMusic("Music");
    ParticleManager::getInstance()->shutdown();
        m_gen.shutdown();

    GameState::exit();
}

} // namespace mmt_gd
