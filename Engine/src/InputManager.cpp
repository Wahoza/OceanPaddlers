#include "stdafx.h"

#include "InputManager.hpp"

namespace mmt_gd
{
InputManager& InputManager::getInstance()
{
    static InputManager instance;
    return instance;
}

void InputManager::process(const sf::Event& event)
{
    switch (event.type) // NOLINT(clang-diagnostic-switch-enum)
    {
        case sf::Event::KeyPressed:
            m_eventFrame.m_keys[event.key.code] = true;
            break;
        case sf::Event::KeyReleased:
            m_eventFrame.m_keys[event.key.code] = false;
            break;
        default:
            break;
    }
}

void InputManager::update()
{
    m_lastFrame    = m_currentFrame;
    m_currentFrame = m_eventFrame;
}

void InputManager::bind(const std::string& action, const int keyCode, const int playerIdx)
{
    ffAssertMsg(playerIdx < PlayerCount, "player out of bounds") m_actionBinding[playerIdx][action] = keyCode;
}

void InputManager::unbind(const std::string& action, const int playerIdx)
{
    ffAssertMsg(playerIdx < PlayerCount, "player out of bounds") m_actionBinding[playerIdx].erase(action);
}

int InputManager::getKeyForAction(const std::string& action, const int playerIdx)
{
    ffAssertMsg(playerIdx < PlayerCount, "player out of bounds")

        const auto it = m_actionBinding[playerIdx].find(action);
    if (it != m_actionBinding[playerIdx].end())
    {
        return it->second;
    }
    return 0;
}

bool InputManager::isKeyDown(const std::string& action, const int playerIdx)
{
    return isKeyDown(getKeyForAction(action, playerIdx));
}

bool InputManager::isKeyUp(const std::string& action, const int playerIdx)
{
    return isKeyUp(getKeyForAction(action, playerIdx));
}

bool InputManager::isKeyPressed(const std::string& action, const int playerIdx)
{
    return isKeyPressed(getKeyForAction(action, playerIdx));
}

bool InputManager::isKeyReleased(const std::string& action, const int playerIdx)
{
    return isKeyReleased(getKeyForAction(action, playerIdx));
}

sf::Vector2f InputManager::getMousePosition() const
{
    ffAssertMsg(m_renderWindow != nullptr, "RenderWindow not set for getMousePosition.")

        const auto p = sf::Mouse::getPosition(*m_renderWindow);
    return {static_cast<float>(p.x), static_cast<float>(p.y)};
}
} // namespace mmt_gd
