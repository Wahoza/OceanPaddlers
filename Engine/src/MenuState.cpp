//Author: Radomir Dinic, modified by Kilian Kolb & Ema Selmanovic
//Description: MenuState for the Game. Is also first state initialized
//License: LICENSE.txt
#include "stdafx.h"

#include "MenuState.hpp"

#include "Game.hpp"
#include "GameStateManager.hpp"
#include "InputManager.hpp"

#include "InputObject.hpp"
namespace mmt_gd
{
using namespace std;

void MenuState::init()
{
    PROFILE_FUNCTION();

    if (m_isInit)
    {
        return;
    }

    if (!m_font.loadFromFile("../assets/HudAssets/PixelFont.ttf"))
    {
        sf::err() << "Could not load font\n";
        return;
    }
    m_font.setSmooth(false);
    m_text.setPosition(m_game->getWindow().getView().getCenter());
    m_text.setString(" <Play>\n Scores");
    m_text.setFillColor(sf::Color::White);
    m_text.setFont(m_font);
    sf::FloatRect textRect = m_text.getLocalBounds();
    m_text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);

    m_view = m_game->getWindow().getView();

    if (!m_backgroundTexture.loadFromFile("../assets/Test.png"))
    {
        sf::err() << "Could not load background\n";
        return;
    }
    m_background.setOrigin(static_cast<float>(m_backgroundTexture.getSize().x) / 2,
                           static_cast<float>(m_backgroundTexture.getSize().y) / 2);

    m_background.setScale(m_game->getWindow().getSize().x / static_cast<float>(m_backgroundTexture.getSize().x),
                          m_game->getWindow().getSize().y / static_cast<float>(m_backgroundTexture.getSize().y));
    m_background.setPosition(m_game->getWindow().getView().getCenter());

    m_background.setTexture(m_backgroundTexture);

    if (!m_logoTexture.loadFromFile("../assets/MMTicon.png"))
    {
        sf::err() << "Could not load icon\n";
        return;
    }
    m_logo.setTexture(m_logoTexture);
    m_logo.setScale(m_game->getWindow().getSize().x / static_cast<float>(m_backgroundTexture.getSize().x),
                          m_game->getWindow().getSize().y / static_cast<float>(m_backgroundTexture.getSize().y));
    m_logo.setScale(m_logo.getScale() / 4.f);
    m_isInit = true;
}

void MenuState::update(float delta)
{
    PROFILE_FUNCTION();

    m_game->getWindow().setView(m_view);

    if (InputManager::getInstance().isKeyPressed("Select") || sf::Joystick::isButtonPressed(0, 0) ||
        sf::Joystick::isButtonPressed(1, 0))
    {
        if (currentSelection == 0)
        {
            m_gameStateManager->setState("MainState");
        }
        else
        {
            m_gameStateManager->setState("HighScoresState");
        }
    }

    currentyAxis = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);
    if (InputManager::getInstance().isKeyDown("up", 0) || InputManager::getInstance().isKeyDown("up", 1))
    {
        currentyAxis += -100;
    }
    if (InputManager::getInstance().isKeyDown("down", 0) || InputManager::getInstance().isKeyDown("down", 1))
    {
        currentyAxis += 100;
    }
    //todo: for holding direction: if held long enough switch to next, decrease waittime for next switch->itterate through many letters with ease
    if (abs(currentyAxis) >= 90 && abs(lastyAxis) <= 90)
    {
        //use to select next or previous char
        int direction = int(currentyAxis / abs(currentyAxis));
        currentSelection += direction;
        if (currentSelection >= 2)
        {
            currentSelection = 0;
        }
        if (currentSelection < 0)
        {
            currentSelection = 1;
        }

        if (currentSelection == 0)
        {
            m_text.setString(" <Play>\n Scores");
        }
        else
        {
            m_text.setString("  Play\n<Scores>");
        }
    }

    lastyAxis = currentyAxis;

    InputObject::getInstance()->StopListen();
}

void MenuState::draw()
{
    PROFILE_FUNCTION();
    m_game->getWindow().clear();
    m_game->getWindow().draw(m_background);
    m_game->getWindow().draw(m_text);
    m_game->getWindow().draw(m_logo);
    m_game->getWindow().display();
}

void MenuState::exit()
{
    PROFILE_FUNCTION();

    GameState::exit();
}
} // namespace mmt_gd
