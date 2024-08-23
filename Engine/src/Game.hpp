//Author: Radomir Dinic, modified by Kilian Kolb
//Description: Managing State for the Game, can switch Gamestate.
//License: LICENSE.txt
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
        std::string  m_windowName = "OceanPaddlers";
    };

    Config& getConfig()
    {
        return m_config;
    }

    static sf::RenderWindow& getWindow();
    static sf::RenderWindow* getWindowPtr();


    void run();

private:
    bool init();
    void initInputManager();
    void update();
    void draw();
    void shutdown() const;

    Config m_config;

    static sf::RenderWindow m_window;
    GameStateManager m_gameStateManager;

    InputManager* m_inputManager = nullptr;
    DebugDraw*    m_debugDraw    = nullptr;
    Fps           m_fps;
};
} // namespace mmt_gd
