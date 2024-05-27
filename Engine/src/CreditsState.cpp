#include "stdafx.h"
#include "CreditsState.hpp"
#include "Game.hpp"
#include "GameStateManager.hpp"
#include "InputManager.hpp"

namespace mmt_gd
{
void CreditsState::init()
{
    PROFILE_FUNCTION();
    initButtonsDown = true;
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
    
    float offset = 0;
    for(auto& str : creditsStrings)
    {
        sf::Text m_text;

        m_text.setPosition(m_game->getWindow().getView().getCenter() + sf::Vector2f{0, offset});
        m_text.setString(str);
        m_text.setFillColor(sf::Color::White);
        m_text.setFont(m_font);
        sf::FloatRect textRect = m_text.getLocalBounds();
        m_text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);

        offset += offsetPerText + textRect.height;

        m_CreditText.push_back(m_text);
    }
    
    offset += 2 * offsetPerText;
    sf::Text backText;
    backText.setPosition(m_game->getWindow().getView().getCenter() + sf::Vector2f{0, offset});
    backText.setString(BackButton);
    backText.setFillColor(sf::Color::White);
    backText.setFont(m_font);
    sf::FloatRect textRect = backText.getLocalBounds();
    backText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);

    m_CreditText.push_back(backText);

    if (!m_backgroundTexture.loadFromFile("../assets/Test.png"))
    {
        sf::err() << "Could not load background\n";
        return;
    }
    m_backgroundSprite.setOrigin(static_cast<float>(m_backgroundTexture.getSize().x) / 2,
                           static_cast<float>(m_backgroundTexture.getSize().y) / 2);

    m_backgroundSprite.setScale(m_game->getWindow().getSize().x / static_cast<float>(m_backgroundTexture.getSize().x),
                          m_game->getWindow().getSize().y / static_cast<float>(m_backgroundTexture.getSize().y));
    m_backgroundSprite.setPosition(m_game->getWindow().getView().getCenter());

    m_backgroundSprite.setTexture(m_backgroundTexture);

    if (!m_iconTexture.loadFromFile("../assets/MMTicon.png"))
    {
        sf::err() << "Could not load icon\n";
        return;
    }
    m_icon.setTexture(m_iconTexture);
    m_icon.setScale(m_game->getWindow().getSize().x / static_cast<float>(m_backgroundTexture.getSize().x),
                    m_game->getWindow().getSize().y / static_cast<float>(m_backgroundTexture.getSize().y));
    m_icon.setScale(m_icon.getScale() / 4.f);

    m_isInit = true;
}
void CreditsState::update(float delta)
{
    PROFILE_FUNCTION();

    if (InputManager::getInstance().isKeyPressed("Select") || sf::Joystick::isButtonPressed(0, 0) ||
        sf::Joystick::isButtonPressed(1, 0))
    {
        if (!initButtonsDown)
        {
            m_gameStateManager->setState("MenuState");
        }
    }
    else
    {
        initButtonsDown = false;
    }
}
void CreditsState::draw()
{
    PROFILE_FUNCTION();
    m_game->getWindow().draw(m_backgroundSprite);
    m_game->getWindow().draw(m_icon);

    for (auto& CreditText : m_CreditText)
    {
        m_game->getWindow().draw(CreditText);
    }
};
void CreditsState::exit()
{
    PROFILE_FUNCTION();
}
}