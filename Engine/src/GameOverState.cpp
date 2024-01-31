#include "stdafx.h"
#include "GameOverState.hpp"
#include "GameState.hpp"
#include "Game.hpp"
#include "EventBus.hpp"
#include "GameObjectEvents.hpp"
namespace mmt_gd
{
using namespace std;

GameOverState::GameOverState(GameStateManager* gameStateManager, Game* game) :
GameState(gameStateManager, game)
{
    EventBus::getInstance().addListener(GameOverEvent::Type, [this](IEvent::Ptr event) { updateStats(event); });
}

void GameOverState::init()
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
    m_text.setString("Game Over.");
    m_text.setFillColor(sf::Color::Red);
    if (won)
    {
        m_text.setString("Victory");
        m_text.setFillColor(sf::Color::Blue);
    }
    
    m_text.setFont(m_font);
    m_text.setOrigin(m_text.getLocalBounds().width * 0.5F, m_text.getLocalBounds().height * 0.5F);
    m_text.setScale(5, 5);

    m_eggsText.setPosition(m_game->getWindow().getView().getCenter());
    m_eggsText.setFillColor(sf::Color::Red);
    if (won)
    {
        m_eggsText.setFillColor(sf::Color::Blue);
    }

    m_eggsText.setFont(m_font);
    m_eggsText.setOrigin(m_text.getLocalBounds().width * 0.5F -150, m_text.getLocalBounds().height * 0.5F -120);
    m_eggsText.setString("Eggs collected: " + to_string(eggsCollected));

    m_distanceText.setPosition(m_game->getWindow().getView().getCenter());
    m_distanceText.setFillColor(sf::Color::Red);
    if (won)
    {
        m_distanceText.setFillColor(sf::Color::Blue);
    }

    m_distanceText.setFont(m_font);
    m_distanceText.setOrigin(m_text.getLocalBounds().width * 0.5F - 150, m_text.getLocalBounds().height * 0.5F - 160);
    m_distanceText.setString("Distance traveled: " + to_string((int)distance) + "m");


    m_view = m_game->getWindow().getView();

    m_isInit = true;
}

void GameOverState::update(float delta)
{
    PROFILE_FUNCTION();
    m_view.setSize(1920, 1080);
    m_game->getWindow().setView(m_view);
}

void GameOverState::draw()
{
    PROFILE_FUNCTION();

    m_game->getWindow().draw(m_text);
    m_game->getWindow().draw(m_eggsText);
    m_game->getWindow().draw(m_distanceText);
}

void GameOverState::updateStats(IEvent::Ptr event)
{
    if (auto gameOverevent = std::dynamic_pointer_cast<GameOverEvent>(event))
    {
        distance = std::get<0>(gameOverevent.get()->getData());

        eggsCollected = std::get<1>(gameOverevent.get()->getData());
        won = std::get<2>(gameOverevent.get()->getData());
    }
}

void GameOverState::exit()
{
    PROFILE_FUNCTION();

    GameState::exit();
}
}