//Author: Kilian Kolb
//Description: Singelton, that displayes Player information on the screen (health, Collectibles collected). Note: This HUD is hardcoded
//License: LICENSE.txt
#include "stdafx.h"
#include "Hud.hpp"
#include "EventBus.hpp"
#include "GameObjectEvents.hpp"
#include "TileNames.hpp"

#include <random>

using namespace sf;
using namespace std;

namespace mmt_gd
{
Hud* Hud::m_instance;
Hud* Hud::getInstance()
{
    if (m_instance == nullptr)
    {  
        m_instance = new Hud();
        //m_instance->CreateHud();
        //auto Notify() = EventBus::EventListener();
    }
    
    return m_instance;
}
Hud::Hud() : EggScore(0), currentlives(3)
{
    EventBus::getInstance().addListener(ScoreEvent::Type, [this](IEvent::Ptr event) { Notify(event); });
    EventBus::getInstance().addListener(EggEvent::Type, [this](IEvent::Ptr event) { Notify(event); });
    EventBus::getInstance().addListener(HurtEvent::Type, [this](IEvent::Ptr event) { Notify(event); });
    EventBus::getInstance().addListener(SwapTiles::Type, [this](IEvent::Ptr event) { Notify(event); });
}
void Hud::Notify(IEvent::Ptr event)
{
    //std::cout << "Notification" << std::endl;
    if (auto eggEvent = std::dynamic_pointer_cast<EggEvent>(event))
    {
        EggScore = eggEvent.get()->getData();
        //std::cout << "eggAlert" << std::endl;
        text.setString("x" + to_string(EggScore));
    }
    if (auto hurtEvent = std::dynamic_pointer_cast<HurtEvent>(event))
    {
        currentlives = hurtEvent.get()->getData();
    }

    if (auto swapEvent = std::dynamic_pointer_cast<SwapTiles>(event))
    {
        std::mt19937 gen(seed + CurrentXPos);
        std::uniform_int_distribution<> distribution(0, 99);

        int randomNumber = distribution(gen);
        int tensDigit = randomNumber / 10;
        int onesDigit = randomNumber % 10;

        transitionSequence = true;
        transitionTime     = 0;
        transitionDuration = std::get<1>(swapEvent.get()->getData());

        int swapType =  std::get<0>(swapEvent.get()->getData());

        swapText.setString(
            "Level " + std::to_string(levelIterator) + ": " + TileNames::getLevelAdjective(swapType, tensDigit) +
            " " + TileNames::getLevelNoun(swapType, onesDigit));

        swapText.setOrigin({swapText.getString().getSize() * swapText.getCharacterSize() / 2.f, 0});

        levelIterator++;
    }
}

void Hud::CreateHud()
{
    //do stuff for initialization here, currently no sprites, etc.
    {
        EggScore         = 0;
        currentlives     = 3;
    }
    {
        if (!m_font.loadFromFile("../assets/HudAssets/PixelFont.ttf"))
        {
        }
        m_font.setSmooth(false);
        

        text.setFillColor(sf::Color::Green);
        text.setFont(m_font);
        text.setCharacterSize(10);
        text.setString("x" + to_string(EggScore));
        text.setFillColor(sf::Color::White);
        text.setOutlineThickness(1);
        text.setOutlineColor(sf::Color::Black);
    }
    {
        sf::Image image;
        if (!image.loadFromFile("../assets/HudAssets/egg.png"))
        {
            sf::err() << "failed to load egg.png" << std::endl;
        }
        //image.createMaskFromColor(sf::Color::Black);

        m_texture_egg.loadFromImage(image);

        Egg = Sprite();
        Egg.setTexture(m_texture_egg);
        Egg.setScale(1.5, 1.5);
    }
    {
        sf::Image image;
        if (!image.loadFromFile("../assets/HudAssets/heart.png"))
        {
            sf::err() << "failed to load heart.png" << std::endl;
        }
        //image.createMaskFromColor(sf::Color::Black);

        m_texture_heart.loadFromImage(image);

        heart = Sprite();
        heart.setTexture(m_texture_heart);
        //Egg.setPosition(-200, -200);
        heart.setScale(1.5, 1.5);
    }
    {
        TransitionShape.setFillColor(Color::Black);
        swapText.setString("Swapping");
        swapText.setFont(m_font);
        swapText.setScale(0.3, 0.3);
    }
}
void Hud::update(float deltaTime)
{
    if (transitionSequence)
    {
        if (transitionDuration < transitionTime)
        {
            transitionSequence = false;
        }

        transitionTime += deltaTime;
    }
}
void Hud::draw(sf::RenderWindow& m_window)
{
    //std::cout << "Draw" << std::endl;
    Vector2f windowSize    = m_window.getView().getSize();
    Vector2f windowCenter  = m_window.getView().getCenter();
    Vector2f windowTopLeft = windowCenter - Vector2f{windowSize.x / 2, windowSize.y / 2};

    CurrentXPos = windowCenter.x;

    {
        Vector2f TextOffset = {25, 20};
        text.setPosition(windowTopLeft + TextOffset);
        Egg.setPosition(windowTopLeft);

        m_window.draw(Egg);
        m_window.draw(text);
    }
    {
        Vector2f windowTopRight = m_window.getView().getCenter() + Vector2f{m_window.getView().getSize().x / 2, -m_window.getView().getSize().y / 2};
        Vector2f HeartOffset = {heart.getScale().x * heart.getTexture()->getSize().x, 0};
        
        Vector2f NextHeartOffset = {heart.getScale().x * heart.getTexture()->getSize().x/5, 0};
        for (float i = initlives; i >initlives-currentlives; i--)
        {
            heart.setPosition(windowTopRight - HeartOffset -NextHeartOffset*i);
            m_window.draw(heart);
        }
    }
    {
        if (transitionSequence)
        {
            float yOffset = 0;
            if (transitionTime < 0.5)
            {
                yOffset = (1 - transitionTime * 2) * windowSize.y;
            }
            else if (transitionTime > transitionDuration -0.5)
            {
                yOffset = (transitionTime - (transitionDuration - 0.5)) * 2 * windowSize.y;
            }

            TransitionShape.setSize(windowSize);
            TransitionShape.setPosition({windowTopLeft.x, windowTopLeft.y + yOffset});

            swapText.setPosition(windowCenter + Vector2f{0, yOffset});
            m_window.draw(TransitionShape);
            m_window.draw(swapText);
        }
    }
}
void Hud::setSeed(int _seed)
{
    seed = _seed;
}
void Hud::shutdown()
{
    delete m_instance;
    m_instance = nullptr;
}
}
