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
    m_text.setString("Press space to continue");
    m_text.setFillColor(sf::Color::White);
    m_text.setFont(m_font);
    m_text.setOrigin(m_text.getLocalBounds().width * 0.5F, m_text.getLocalBounds().height * 0.5F);

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

    if (InputManager::getInstance().isKeyPressed("Select"))
    {
        m_gameStateManager->setState("MainState");
    }
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
