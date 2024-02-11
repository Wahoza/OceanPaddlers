//Author: Kilian Kolb
//Description: Singelton that reads keyboard inputs and writes them into a box. does not listen to return or comma inputs
//License: LICENSE.txt
#include "stdafx.h"
#include "Game.hpp"
#include "InputObject.hpp"
#include "InputManager.hpp"
#include "EventBus.hpp"
#include "GameObjectEvents.hpp"

namespace mmt_gd
{

InputObject::InputObject() : m_rectangle()
{
}

void InputObject::init(sf::Vector2f position, sf::Vector2f size, Color color)
{
    m_rectangle.setSize(size);
    m_rectangle.setPosition(position);
    m_rectangle.setFillColor(sf::Color::White);
    m_rectangle.setOutlineThickness(2.f);
    m_rectangle.setOutlineColor(color);

    m_text.setFont(m_font);
    m_text.setCharacterSize(20);
    m_text.setFillColor(color);
    m_text.setPosition(position.x + 5.f, position.y + 5.f);
}

void InputObject::update(float deltaTime)
{
    //auto& data = InputManager::getInstance().getEventData();
    //data.
}
void InputObject::handleEvent(sf::Event &event)
{
    if (event.type == sf::Event::TextEntered)
    {   
        if (listening && event.text.unicode < 128)
        {
            if (event.text.unicode == '\b' && !m_inputString.empty())
            {
                m_inputString.pop_back();
                IEvent::Ptr inputEvent = std::make_shared<InputEvent>(event.text.unicode);
                EventBus::getInstance().fireEvent(inputEvent);
            }
            else if (event.text.unicode != '\b' && event.text.unicode != 10 && event.text.unicode != 13 &&
                     event.text.unicode != ',' &&
                     m_inputString.size() <= maxSize)
            {
                m_inputString += static_cast<char>(event.text.unicode);
                IEvent::Ptr inputEvent = std::make_shared<InputEvent>(event.text.unicode);
                EventBus::getInstance().fireEvent(inputEvent);
            }
            updateText();
        }
        //std::cout << m_inputString << std::endl;
    }
}
void InputObject::StartListen()
{
    listening = true;
}
void InputObject::StopListen()
{
    listening = false;
}
string InputObject::getText()
{
    return m_inputString;
}
void InputObject::updateText()
{
    m_text.setString(m_inputString);
}
InputObject* InputObject::m_instance = nullptr;
InputObject*  InputObject::getInstance()
{
    if (m_instance == nullptr)
    {
        m_instance = new InputObject();

        Font TempFont;
        if(!TempFont.loadFromFile("../assets/HudAssets/PixelFont.ttf"))
        {
            sf::err() << "Could not load font\n";
        }
        m_instance->m_font = TempFont;
    }
    return m_instance;
}
void InputObject::shutdown()
{
    delete m_instance;
    m_instance = nullptr;
}
}