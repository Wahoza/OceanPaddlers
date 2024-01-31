#include "stdafx.h";
#include "ScoreDisplayComponent.hpp"
#include "IComponent.hpp"
#include "IRenderComponent.hpp"
#include "GameObject.hpp"
#include "Game.hpp"
#include "EventBus.hpp"
#include "RenderComponentEvents.hpp"
using namespace sf;
using namespace std;
namespace mmt_gd
{
ScoreDisplayComponent::ScoreDisplayComponent(GameObject& gameObject, RenderWindow& renderWindow, string _text, Vector2f position) :
IRenderComponent(gameObject, renderWindow)
{
    EventBus::getInstance().fireEvent(std::make_shared<RenderableCreateEvent>("Top", *this));
    
    if (!m_font.loadFromFile("../assets/HudAssets/PixelFont.ttf"))
    {
        sf::err() << "Could not load font\n";
    }
    m_font.setSmooth(false);
    m_text.setFillColor(sf::Color::Green);
    m_text.setFont(m_font);
    m_text.setCharacterSize(10);
    m_text.setString(_text);
    m_text.setFillColor(sf::Color::White);
    m_text.setOutlineThickness(1);
    m_text.setOutlineColor(sf::Color::Black);
    m_text.setOrigin(position);
}

bool ScoreDisplayComponent::init()
{
    return true;
}

void ScoreDisplayComponent::update(float deltaTime)
{
}

void ScoreDisplayComponent::draw()
{
    m_renderWindow.draw(m_text);
}
ScoreDisplayComponent::~ScoreDisplayComponent()
{
}
} // namespace mmt_gd 