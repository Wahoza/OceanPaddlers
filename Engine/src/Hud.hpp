#pragma once
//Note: this is probably noth the ideal solution, but i think its best if this is completely seperate (as it is for a very specific case)
//Idea: i can alway put it in the render /update after all the game object/sprites consistantly
#include "stdafx.h"
#include "EventBus.hpp"

using namespace sf;
using namespace std;

namespace mmt_gd
{
class Hud
{
public:
    static Hud* getInstance();
    void        shutdown();
    void        CreateHud();
    Hud();
    void Notify(IEvent::Ptr event);

    void PingGameEnd();
    
    //void IEvent::getEventType
    //list<Sprite, Vector2f> spriteList;

    //EventBus::EventListener listener;
    //using Ptr = std::shared_ptr<Hud>;

    View m_view;
    int   scoreP1 = 0;
    int   scoreP2 = 0;
    void  update(float deltaTime);
    void  draw(sf::RenderWindow& m_window);
    
    string mainText = "Score: P1 - P2";
    Vector2f offset   = {-400, -400};

    Text* text;
    Font m_font;

    Texture m_texture_egg;
    Sprite Egg;

    Texture m_texture_heart;
    Sprite  heart;

    int EggScore = 0;
    int initlives    = 3;
    int currentlives = 3;
    bool won          = false;

private:
    string        m_layerName;
    static Hud* m_instance;
    float         distanceTraveled = 0;
    float         distancePixeltoM = 30;
};
} // namespace mmt_gd