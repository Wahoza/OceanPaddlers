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
    float spacing = 1.0;
    int   spaceSize     = size.x / maxSize / 6 * spacing;
    int   characterSize = (size.x / maxSize - spaceSize);
    
    size.x = maxSize * characterSize + maxSize * spaceSize;
    //note: the size.x is now no longer the original size but fairly close and ligns up with the closest possible character size.

    m_rectangle.setSize({size.x, size.y});
    m_rectangle.setPosition(position);
    m_rectangle.setFillColor(sf::Color::White);
    m_rectangle.setOutlineThickness(2.f);
    m_rectangle.setOutlineColor(color);

    sf::Color IndicatorColor = color;
    IndicatorColor.a         = 100;
    m_CurrentCharIndicator.setSize({(size.x / (maxSize)), size.y});
    m_CurrentCharIndicator.setPosition({position.x, position.y});
    m_CurrentCharIndicator.setFillColor(IndicatorColor);
    m_CurrentCharIndicator.setOutlineThickness(0.f);

    m_font.setSmooth(false);
    m_text.setFont(m_font);

    m_text.setCharacterSize(characterSize);
    m_text.setFillColor(color);
    m_text.setPosition(position.x + spaceSize/2, position.y + size.y / 10);
    m_text.setLetterSpacing(spacing); //note: spacing works as a pixel value scaled up with character size

    for (int i = 0; i <= 25; i++)
    {
        m_IntCharMap[i] = char(i + 65);
    }
    for (int i = 0; i <= 9; i++)
    {
        m_IntCharMap[i+26] = char(i + 48);
    }
    int AddedVals = 36; //Magic number for chars in alphabelt + numbers
    
    m_IntCharMap[0 + AddedVals] = '(';
    m_IntCharMap[1 + AddedVals] = ')';
    m_IntCharMap[2 + AddedVals] = ';';
    m_IntCharMap[3 + AddedVals] = ':';
    m_IntCharMap[4 + AddedVals] = '.';
    m_IntCharMap[5 + AddedVals] = '&';
    m_IntCharMap[6 + AddedVals] = '%';
    m_IntCharMap[7 + AddedVals] = '=';
    m_IntCharMap[8 + AddedVals] = '!';
    m_IntCharMap[9 + AddedVals] = '?';
    m_IntCharMap[10 + AddedVals] = ' ';

    maxCharInt = AddedVals + 10;
    
    if (!m_InputTexture.loadFromFile("../assets/selectButton.png"))
    {
        sf::err() << "Could not load icon\n";
        return;
    }
    m_InputSprite.setTexture(m_InputTexture);
    m_InputSprite.setScale(size.y / m_InputTexture.getSize().y, size.y / m_InputTexture.getSize().y);
    m_InputSprite.setPosition(position.x + size.x + 5, position.y);
    m_InputSprite.setTextureRect({0, 0, int(m_InputTexture.getSize().x / 2), int(m_InputTexture.getSize().y)});
    m_InputSprite.setColor(color);
}

void InputObject::update(float deltaTime)
{
    handleController(deltaTime);
}
void InputObject::handleEvent(sf::Event& event)
{
    handleText(event);
}

void InputObject::handleController(float deltaTime)
{
    currentyAxis = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);
    currentxAxis = sf::Joystick::getAxisPosition(0, sf::Joystick::X);

    //todo: for holding direction: if held long enough switch to next, decrease waittime for next switch->itterate through many letters with ease
    if (abs(currentxAxis) >= 90 && abs(lastxAxis) <= 90)
    {
        m_InputSprite.setTextureRect({0, 0, int(m_InputTexture.getSize().x / 2), int(m_InputTexture.getSize().y)});

        //use to select next or previous char
        int direction = int(currentxAxis / abs(currentxAxis));
        currentCharPos  += direction;
        if (currentCharPos >= maxSize)
        {
            currentCharPos = 0;
        }
        if (currentCharPos < 0)
        {
            currentCharPos = maxSize-1;
        }

        Vector2f RectPos = m_rectangle.getPosition();
        Vector2f NewCharIndikatorPos =
            {(RectPos.x + (m_CurrentCharIndicator.getSize().x) * currentCharPos), RectPos.y};
        m_CurrentCharIndicator.setPosition(NewCharIndikatorPos);
    }


    if (abs(currentyAxis) >= 90 && abs(lastyAxis) <= 90)
    {
        //use to iterate through chars
        bufferTime = 1; // only required here
        updateStringChar(currentCharPos, -1 * int(currentyAxis / abs(currentyAxis)));
    }
    else if (abs(currentyAxis) >= 90 )
    {
        timeHeld += deltaTime;

        if (timeHeld > bufferTime)
        {
            bufferTime = bufferTime * 0.5;
            if (bufferTime < 0.1)
            {
                bufferTime = 0.1;
            }
            timeHeld   = 0;

            updateStringChar(currentCharPos, -1 * int(currentyAxis / abs(currentyAxis)));
        }
    }

    lastxAxis = currentxAxis;
    lastyAxis = currentyAxis;
}
void InputObject::updateStringChar(int position, int direction)
{
    if (position >= m_inputString.size())
    {
        m_inputString.resize(position + 1, ' '); // Fill new positions with space
    } 
    auto charIterator = std::find_if(m_IntCharMap.begin(),
                                     m_IntCharMap.end(),
                                     [currentChar = m_inputString[position]](const auto& pair)
                                     { return pair.second == currentChar; });
    int currentChar = 0;
    if (charIterator == m_IntCharMap.end())
    {
        m_inputString[position] = ' '; //contained within map
        currentChar             = maxCharInt;
    }
    else
    {
        currentChar = charIterator->first;
    }

    currentChar += direction;
    if (currentChar > maxCharInt)
    {
        currentChar = 0;
    }
    if (currentChar < 0)
    {
        currentChar = maxCharInt;
    }

    m_inputString[position] = m_IntCharMap[currentChar];
    updateText();

    IEvent::Ptr inputEvent = std::make_shared<InputEvent>(m_IntCharMap[currentChar]);
    EventBus::getInstance().fireEvent(inputEvent);
}
void InputObject::handleText(sf::Event& event)
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
                     event.text.unicode != ',' && event.text.unicode != '\t' && m_inputString.size() <= maxSize - 1)
            {
                m_inputString += static_cast<char>(event.text.unicode);
                IEvent::Ptr inputEvent = std::make_shared<InputEvent>(event.text.unicode);
                EventBus::getInstance().fireEvent(inputEvent);

                 m_InputSprite.setTextureRect({int(m_InputTexture.getSize().x / 2),
                                              0,
                                              int(m_InputTexture.getSize().x / 2),
                                              int(m_InputTexture.getSize().y)});
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
void InputObject::draw(RenderWindow& window)
{
    window.draw(m_rectangle);
    window.draw(m_CurrentCharIndicator);
    window.draw(m_text);
    window.draw(m_InputSprite);
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