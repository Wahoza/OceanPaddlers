#include "stdafx.h"

#include "InputObject.hpp"

#include "GameOverState.hpp"
#include "GameState.hpp"
#include "Game.hpp"
#include "EventBus.hpp"
#include "GameObjectEvents.hpp"
#include "InputManager.hpp"

using namespace std;
using namespace sf;

namespace mmt_gd
{

GameOverState::GameOverState(GameStateManager* gameStateManager, Game* game) : GameState(gameStateManager, game), wait(0)
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
    m_text.setScale(5, 5);

    m_eggsText.setPosition(ScreenCenter);
    m_eggsText.setFillColor(sf::Color::Red);
    if (won)
    {
        m_eggsText.setFillColor(sf::Color::Blue);
    }

    m_eggsText.setFont(m_font);
    m_eggsText.setOrigin(m_text.getLocalBounds().width * 0.5F -150, m_text.getLocalBounds().height * 0.5F -120);
    m_eggsText.setString("Eggs collected: " + to_string(eggsCollected));

    m_distanceText.setPosition(ScreenCenter);
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
    InputObject::getInstance()->init(ScreenCenter + Vector2f(-400, 120),
                                     {200, 30},
                                     color);
    //InputObject::getInstance()->StartListen();
}

void GameOverState::update(float delta)
{
    PROFILE_FUNCTION();
    m_view.setSize(1920, 1080);
    m_game->getWindow().setView(m_view);
    wait += delta;
    if (wait > 0.1)
    {
        InputObject::getInstance()->StartListen();
    }
    if (InputManager::getInstance().isKeyPressed("Exit"))
    {
        m_gameStateManager->setState("MenuState");
    }
}

void GameOverState::draw()
{
    PROFILE_FUNCTION();

    m_game->getWindow().draw(m_text);
    m_game->getWindow().draw(m_eggsText);
    m_game->getWindow().draw(m_distanceText);

    m_game->getWindow().draw(InputObject::getInstance()->m_rectangle);
    m_game->getWindow().draw(InputObject::getInstance()->m_text);
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
    {
        std::string filePath = "../assets/scores.txt";

        // Read the existing file contents
        std::ifstream inputFile(filePath);
        if (inputFile.is_open())
        {
            std::string fileContent((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
            inputFile.close();

            // Append new data to the existing content
            fileContent +="\n" + InputObject::getInstance()->getText() + "," + to_string((int)distance) +
                               "," +
                               to_string(eggsCollected);

            // Write the updated content back to the file
            std::ofstream outputFile(filePath);
            if (outputFile.is_open())
            {
                outputFile << fileContent;
                outputFile.close();
                std::cout << "File updated successfully." << std::endl;
            }
            else
            {
                std::cerr << "Error opening file for writing: " << filePath << std::endl;
            }
        }
        else
        {
            std::cerr << "Error opening file for reading: " << filePath << std::endl;
        }
    }
    GameState::exit();
    InputObject::getInstance()->shutdown();
}
}