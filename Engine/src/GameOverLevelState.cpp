//Author: Kilian Kolb
//Description: Game State for the End of the Game. Contains posibility for victory screen or game over Screen. utilizes InputObject.
//License: LICENSE.txt
#pragma once
#include "stdafx.h"

#include "InputObject.hpp"

#include "GameOverLevelState.hpp"
#include "GameState.hpp"
#include "Game.hpp"
#include "EventBus.hpp"
#include "GameObjectEvents.hpp"
#include "InputManager.hpp"
#include "SoundManager.hpp"

using namespace std;
using namespace sf;

namespace mmt_gd
{

GameOverLevelState::GameOverLevelState(GameStateManager* gameStateManager, Game* game) :
GameState(gameStateManager, game)
{
    EventBus::getInstance().addListener(GameOverEvent::Type, [this](IEvent::Ptr event) { updateStats(event); });
}

void GameOverLevelState::init()
{
    PROFILE_FUNCTION();


    if (!m_font.loadFromFile("../assets/HudAssets/PixelFont.ttf"))
    {
        sf::err() << "Could not load font\n";
        return;
    }
    m_font.setSmooth(false);
    
    Vector2f ScreenCenter = m_game->getWindow().getView().getCenter();
    m_text.setPosition(ScreenCenter);
    m_text.setString("Game Over.");
    sf::Color color = sf::Color::Red;
    
    if (won)
    {
        m_text.setString("Victory");
        color = sf::Color::Blue;
    }

    m_text.setFillColor(color);
    m_text.setFont(m_font);
    m_text.setOrigin(m_text.getLocalBounds().width * 0.5F, m_text.getLocalBounds().height * 0.5F);
    m_text.setPosition(ScreenCenter.x, ScreenCenter.y - 100);
    m_text.setScale(5, 5);

    m_eggsText.setPosition(ScreenCenter.x - 700, ScreenCenter.y + 100);
    m_eggsText.setFillColor(color);

    m_eggsText.setFont(m_font);
    m_eggsText.setString("Eggs collected: " + to_string(eggsCollected));

    m_distanceText.setPosition(ScreenCenter.x - 700, ScreenCenter.y+150);
    m_distanceText.setFillColor(color);

    m_distanceText.setFont(m_font);
    m_distanceText.setString("Distance traveled: " + to_string((int)distance) + "m");


    m_totalScoreText.setFont(m_font);
    m_totalScoreText.setPosition(ScreenCenter.x - 700, ScreenCenter.y + 200);
    m_totalScoreText.setFillColor(color);
    m_totalScoreText.setString("Total score: " + to_string(totalScore));

    m_view = m_game->getWindow().getView();

    //InputObject::getInstance()->StartListen();
}

void GameOverLevelState::update(float delta)
{
    PROFILE_FUNCTION();
    m_view.setSize(1920, 1080);
    m_game->getWindow().setView(m_view);
  
    if (InputManager::getInstance().isKeyPressed("Exit") || InputManager::getInstance().isKeyPressed("Continue") ||
        sf::Joystick::isButtonPressed(0, 0) || sf::Joystick::isButtonPressed(1, 0))
    {
        m_gameStateManager->setState("MenuState");
    }
}

void GameOverLevelState::draw()
{
    PROFILE_FUNCTION();

    m_game->getWindow().draw(m_text);
    m_game->getWindow().draw(m_eggsText);
    m_game->getWindow().draw(m_distanceText);
    m_game->getWindow().draw(m_totalScoreText);

    m_game->getWindow().draw(m_name);
    //m_game->getWindow().draw(InputObject::getInstance()->m_rectangle);
    //m_game->getWindow().draw(InputObject::getInstance()->m_text);
}

void GameOverLevelState::updateStats(IEvent::Ptr event)
{
    if (auto gameOverevent = std::dynamic_pointer_cast<GameOverEvent>(event))
    {
        distance = std::get<0>(gameOverevent.get()->getData());

        eggsCollected = std::get<1>(gameOverevent.get()->getData());
        won = std::get<2>(gameOverevent.get()->getData());
        totalScore = std::get<3>(gameOverevent.get()->getData());
    }
}

void GameOverLevelState::exit()
{
    PROFILE_FUNCTION();

    GameState::exit();

}
}