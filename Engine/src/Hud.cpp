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
        m_instance->CreateHud();
        //auto Notify() = EventBus::EventListener();
    }
    
    return m_instance;
}
Hud::Hud()
{
    EventBus::getInstance().addListener(ScoreEvent::Type, [this](IEvent::Ptr event) { Notify(event); });
}
void Hud::Notify(IEvent::Ptr event)
{
    //std::cout << "Notification" << std::endl;
    if (auto scoreEvent = std::dynamic_pointer_cast<ScoreEvent>(event))
    {
        int score = scoreEvent->getData().first;
        //std::cout << score << std::endl;
        string player = scoreEvent->getData().second;
        if (player == "Score P1")
        {
            scoreP1 += score;
        }
        else
        {
            scoreP2 += score;
        }
        text->setString("P1 " + to_string(scoreP1) + " - P2 " + to_string(scoreP2));
    }
}
void Hud::CreateHud()
{
    //do stuff for initialization here, currently no sprites, etc.
    if (!m_font.loadFromFile("../assets/consolab.ttf"))
    {
        sf::err() << "Could not load font\n";
    }
    text = new Text();
    text->setPosition(-100, -100);
    text->setString("P1 " + to_string(scoreP1) + " - P2 " + to_string(scoreP2));
    text->setFillColor(sf::Color::Green);
    text->setFont(m_font);
    text->setCharacterSize(60);
}
void Hud::update(float deltaTime)
{
    
}
void Hud::draw(sf::RenderWindow& m_window)
{
    //std::cout << "Draw" << std::endl;
    Vector2f windowTopLeft = m_window.getView().getCenter() - Vector2f{m_window.getView().getSize().x / 2, m_window.getView().getSize().y / 2};
    text->setPosition(windowTopLeft);
    m_window.draw(*text);
}
void Hud::shutdown()
{
    delete text;
    delete m_instance;
}
}
