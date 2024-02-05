#include "stdafx.h"
#include "Hud.hpp"
#include "EventBus.hpp"
#include "GameObjectEvents.hpp"
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
Hud::Hud() : EggScore(0), distanceTraveled(0), currentlives(3)
{
    EventBus::getInstance().addListener(ScoreEvent::Type, [this](IEvent::Ptr event) { Notify(event); });
    EventBus::getInstance().addListener(EggEvent::Type, [this](IEvent::Ptr event) { Notify(event); });
    EventBus::getInstance().addListener(HurtEvent::Type, [this](IEvent::Ptr event) { Notify(event); });
}
void Hud::Notify(IEvent::Ptr event)
{
    //std::cout << "Notification" << std::endl;
    if (auto eggEvent = std::dynamic_pointer_cast<EggEvent>(event))
    {
        EggScore += 1;
        //std::cout << "eggAlert" << std::endl;
        text.setString("x" + to_string(EggScore));
    }
    if (auto hurtEvent = std::dynamic_pointer_cast<HurtEvent>(event))
    {
        currentlives--;
        if (currentlives == 0)
        {
            PingGameEnd();
            currentlives = 3;
        }
    }
}
void Hud::PingGameEnd()
{
    tuple       eventData(distanceTraveled, EggScore, won);
    IEvent::Ptr GameOver = std::make_shared<GameOverEvent>(eventData);
    EventBus::getInstance().fireEvent(GameOver);
}
void Hud::CreateHud()
{
    //do stuff for initialization here, currently no sprites, etc.
    {
        distanceTraveled = 0;
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
        //Egg.setPosition(-200, -200);
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
}
void Hud::update(float deltaTime)
{
    
}
void Hud::draw(sf::RenderWindow& m_window)
{
    //std::cout << "Draw" << std::endl;
    {
        distanceTraveled = m_window.getView().getCenter().x / distancePixeltoM;


        Vector2f windowTopLeft = m_window.getView().getCenter() -
                                 Vector2f{m_window.getView().getSize().x / 2, m_window.getView().getSize().y / 2};
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
}
void Hud::shutdown()
{
    delete m_instance;
    m_instance = nullptr;
}
}
