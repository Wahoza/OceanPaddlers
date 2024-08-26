//Author: Radomir Dinic, modified by Kilian Kolb
//Description: Managing State for the Game, can switch Gamestate.
//License: LICENSE.txt
#include "stdafx.h"

#include "Game.hpp"

#include "DebugDraw.hpp"
#include "InputManager.hpp"
#include "MainEndlessState.hpp"
#include "MainLevelState.hpp"
#include "MenuState.hpp"
#include "GameOverEndlessState.hpp"
#include "GameOverLevelState.hpp"
#include "HighScoresState.hpp"
#include "CreditsState.hpp"
#include "InputObject.hpp"
#include <sstream>
#include "GameObjectManager.hpp"
#include "Hud.hpp"
#include "PhysicsManager.hpp"
#include "AiManager.hpp"

namespace mmt_gd
{
using namespace std;

sf::RenderWindow Game::m_window;

sf::RenderWindow& Game::getWindow()
{
    return m_window;
}
sf::RenderWindow* Game::getWindowPtr()
{
    return &m_window;
}

void Game::run()
{
    Instrumentor::instance().beginSession("startup", "startup.json");

    if (!init())
    {
        return;
    }
    Instrumentor::instance().endSession();
    Instrumentor::instance().beginSession("runtime", "runtime.json");
    m_window.setMouseCursorVisible(false);

    while (m_window.isOpen())
    {
        PROFILE_SCOPE("Frame");

        {
            PROFILE_SCOPE("Process Events");

            // process events in the input manager
            sf::Event event{};
            while (m_window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                {
                    shutdown();
                    m_window.close();
                    return;
                }
                m_inputManager->process(event);
                InputObject::getInstance()->handleEvent(event);
            }
        }
        update();
        draw();
    }

    shutdown();
}

void Game::initInputManager()
{
    m_inputManager = &InputManager::getInstance();

    m_inputManager->bind("Exit", sf::Keyboard::Escape);
    m_inputManager->bind("Select", sf::Keyboard::Space);
    m_inputManager->bind("Continue", sf::Keyboard::Enter);

    // May move to view later on
    m_inputManager->bind("up", sf::Keyboard::W, 0);
    m_inputManager->bind("left", sf::Keyboard::A, 0);
    m_inputManager->bind("down", sf::Keyboard::S, 0);
    m_inputManager->bind("right", sf::Keyboard::D, 0);
    m_inputManager->bind("attack", sf::Keyboard::C, 0);

    m_inputManager->bind("up", sf::Keyboard::Up, 1);
    m_inputManager->bind("left", sf::Keyboard::Left, 1);
    m_inputManager->bind("down", sf::Keyboard::Down, 1);
    m_inputManager->bind("right", sf::Keyboard::Right, 1);
    m_inputManager->bind("attack", sf::Keyboard::RShift, 1);

    m_inputManager->bind("debugdraw", sf::Keyboard::F1, 0);
}

bool Game::init()
{
    PROFILE_FUNCTION();

    initInputManager();

    m_debugDraw = &DebugDraw::getInstance();

    //
    m_gameStateManager.registerState("MenuState", make_shared<MenuState>(&m_gameStateManager, this));
    m_gameStateManager.registerState("MainEndlessState", make_shared<MainEndlessState>(&m_gameStateManager, this));
    m_gameStateManager.registerState("MainLevelState", make_shared<MainLevelState>(&m_gameStateManager, this));
    m_gameStateManager.registerState("GameOverEndlessState", make_shared<GameOverEndlessState>(&m_gameStateManager, this));
    m_gameStateManager.registerState("GameOverLevelState", make_shared<GameOverLevelState>(&m_gameStateManager, this));
    m_gameStateManager.registerState("HighScoresState", make_shared<HighScoreState>(&m_gameStateManager, this));
    m_gameStateManager.registerState("CreditsState", make_shared<CreditsState>(&m_gameStateManager, this));

    //
    m_window.create(sf::VideoMode(m_config.m_resolution.x, m_config.m_resolution.y),
                    m_config.m_windowName,
                    sf::Style::Fullscreen);
    m_window.setFramerateLimit(144);

    m_inputManager->setRenderWindow(&m_window);

    m_gameStateManager.setState("MenuState");

    return true;
}

void Game::update()
{
    PROFILE_FUNCTION();

    // starts the clock
    static sf::Clock clock;
    // restart takes time
    const auto deltaTime        = clock.restart();
    const auto deltaTimeSeconds = deltaTime.asSeconds();

    // must be first call
    m_inputManager->update();

    m_gameStateManager.update(deltaTimeSeconds);

    m_debugDraw->update(deltaTimeSeconds);

    std::ostringstream ss;
    m_fps.update();
    ss << m_config.m_windowName << " | FPS: " << m_fps.getFps();

    m_window.setTitle(ss.str());
}

void Game::draw()
{
    PROFILE_FUNCTION();

    m_window.clear();

    m_gameStateManager.draw();
    
    m_debugDraw->draw(m_window);

    m_window.display();
}

void Game::shutdown() const
{
    m_gameStateManager.shutdown();
    PhysicsManager::getInstance()->shutdown();
    AiManager::getInstance()->shutdown();
    InputObject::getInstance()->shutdown();
}
} // namespace mmt_gd
