#pragma once

#include "FPS.hpp"
#include "GameStateManager.hpp"

#include <TGUI/Backend/SFML-Graphics.hpp>
#include <string>

namespace mmt_gd
{
class InputManager;
class DebugDraw;

class Game
{
public:
    struct Config
    {
        //Window size is here!!
        sf::Vector2i m_resolution{1920, 1080};
        std::string  m_windowName = "FinalFrontier";
    };

    Config& getConfig()
    {
        return m_config;
    }

    sf::RenderWindow& getWindow()
    {
        return m_window;
    }


    void run();

private:
    bool init();
    void initInputManager();
    void update();
    void draw();
    void shutdown() const;

    Config m_config;

    sf::RenderWindow m_window;
    GameStateManager m_gameStateManager;

    InputManager* m_inputManager = nullptr;
    DebugDraw*    m_debugDraw    = nullptr;
    Fps           m_fps;
};
} // namespace mmt_gd
