#pragma once
//Note: this is probably noth the ideal solution, but i think its best if this is completely seperate (as it is for a very specific case)
//Idea: i can alway put it in the render /update after all the game object/sprites consistantly
#include "stdafx.h"
#include "EventBus.hpp"

using namespace sf;
//Author: Kilian Kolb
//Description: Singelton, that displayes Player information on the screen (health, Collectibles collected). Note: This HUD is hardcoded
//License: LICENSE.txt
using namespace std;

namespace mmt_gd
{
class Hud
{
public:
    void        CreateHud();
    Hud();
    void Notify(IEvent::Ptr event);    
    void setSeed(int _seed);
    //void IEvent::getEventType
    //list<Sprite, Vector2f> spriteList;

    //EventBus::EventListener listener;
    //using Ptr = std::shared_ptr<Hud>;

    View m_view;
    int   scoreP1 = 0;
    int   scoreP2 = 0;
    void  update(float deltaTime);
    void  draw(sf::RenderWindow& m_window);
    
    Vector2f offset   = {-400, -400};

    Text text;
    Font m_font;

    Texture m_texture_egg;
    Sprite Egg;

    Texture m_texture_heart;
    Sprite  heart;

    int EggScore = 0;
    int initlives    = 3;
    int currentlives = 3;

    bool won          = false;

    int levelIterator = 0;

private:
    float       CurrentXPos = 0;
    string        m_layerName;

    Text swapText;
    RectangleShape TransitionShape;
    float          transitionTime = 0;
    float          transitionDuration = 3;
    bool           transitionSequence = false;

    int seed;

    sf::Text m_pauseText;
    bool paused = false;
};
} // namespace mmt_gd