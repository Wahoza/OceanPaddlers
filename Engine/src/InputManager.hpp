#pragma once

#include "SFML/Window.hpp"

#include <unordered_map>

namespace mmt_gd
{
class InputManager
{
public:
    static InputManager& getInstance();
    InputManager(const InputManager& rhv)             = delete;
    InputManager(InputManager&& rhv)                  = delete;
    InputManager&  operator=(const InputManager& rhv) = delete;
    InputManager&& operator=(InputManager&& rhv)      = delete;

    /**
     * \brief Must be called at beginning of game update, before the update
    ´* method. The events are
     * classified and processed internally.
     * \param event a single event coming from the sf polling.
     */
    void process(const sf::Event& event);


    /**
     * \brief Must be called after all events have been processed.
     */
    void update();

    /**
     * \brief Binds an action to the given keycode and for the given player
    ´* \param action the name of the action (eg. left, jump, ...)
     * \param keyCode the keycode of the action
     * \param playerIdx the idx of the player
     */
    void bind(const std::string& action, int keyCode, int playerIdx = 0);

    void unbind(const std::string& action, int playerIdx = 0);

    /**
     * \return Returns true if the key button is currently down.
     */
    bool isKeyDown(const int keyCode) const
    {
        ffAssertMsg(keyCode >= 0 && keyCode < sf::Keyboard::KeyCount,
                    "KeyCode out of bounds") return m_currentFrame.m_keys[keyCode];
    }

    /**
     * \return Returns true if the key button is currently up.
     */
    bool isKeyUp(const int keyCode) const
    {
        ffAssertMsg(keyCode >= 0 && keyCode < sf::Keyboard::KeyCount,
                    "KeyCode out of bounds") return !m_currentFrame.m_keys[keyCode];
    }

    /**
     * \return Returns true if the key button has been pressed.
     */
    bool isKeyPressed(const int keyCode) const
    {
        ffAssertMsg(keyCode >= 0 && keyCode < sf::Keyboard::KeyCount,
                    "KeyCode out of bounds") return m_currentFrame.m_keys[keyCode] &&
            !m_lastFrame.m_keys[keyCode];
    }

    /**
     * \return Returns true if the key button has been released.
     */
    bool isKeyReleased(const int key_code) const
    {
        ffAssertMsg(key_code >= 0 && key_code < sf::Keyboard::KeyCount,
                    "KeyCode out of bounds") return !m_currentFrame.m_keys[key_code] &&
            m_lastFrame.m_keys[key_code];
    }

    /**
     * \return Returns true if the button for the given Action is currently down.
     */
    bool isKeyDown(const std::string& action, int playerIdx = 0);

    /**
     * \return Returns true if the button for the given Action is currently up.
     */
    bool isKeyUp(const std::string& action, int playerIdx = 0);

    /**
     * \return Returns true if the button for the given Action has been pressed.
     */
    bool isKeyPressed(const std::string& action, int playerIdx = 0);

    /**
     * \return Returns true if the button for the given Action has been released.
     */
    bool isKeyReleased(const std::string& action, int playerIdx = 0);
    

    /**
     * \return Returns the current mouse position relative to the window
     */
    sf::Vector2f getMousePosition() const;

    void setRenderWindow(sf::RenderWindow* window)
    {
        m_renderWindow = window;
    }

private:
    struct FrameData
    {
        bool m_keys[sf::Keyboard::KeyCount];
    };
    InputManager()  = default;
    ~InputManager() = default;


    int getKeyForAction(const std::string& action, int playerIdx);

    FrameData m_lastFrame{};
    FrameData m_currentFrame{};
    FrameData m_eventFrame{};

    sf::RenderWindow* m_renderWindow{nullptr};

    static constexpr int                 PlayerCount = 4; ///< maximum allowed players. Can be increased if needed.
    std::unordered_map<std::string, int> m_actionBinding[PlayerCount];
};
} // namespace mmt_gd
