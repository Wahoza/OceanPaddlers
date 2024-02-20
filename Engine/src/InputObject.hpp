//Author: Kilian Kolb
//Description: Singelton that reads keyboard inputs and writes them into a box. does not listen to return or comma inputs
//License: LICENSE.txt
#pragma once
#include "stdafx.h"

using namespace sf;
using namespace std;
namespace mmt_gd
{
class InputObject final
{
public:
    static InputObject* getInstance();

    void shutdown();

    void init(Vector2f position, Vector2f size, Color color);
    void handleEvent(const sf::Event& event);
    void update(float deltaTime);

    void handleEvent(sf::Event &event);

    void handleController(float deltaTime);
    void updateStringChar(int position, int direction);

    void handleText(sf::Event& event);
    void draw(RenderWindow& window);


    RectangleShape  m_rectangle;
    Text            m_text;
    void            StartListen();
    void            StopListen();
    string          getText();

private:
    InputObject();

    static InputObject* m_instance;
    int            maxSize = 10;
    void           updateText();
    bool           listening = false;
    string m_inputString = "";
    Font           m_font;

    //controller related
    float currentxAxis = 0;
    float currentyAxis = 0;
    float lastxAxis    = 0;
    float lastyAxis    = 0;
    float timeHeld     = 0;
    float bufferTime   = 0;
    //note:easier use of special characters, requires initialization
    int currentCharPos = 1;
    int maxCharInt = 0;
    std::unordered_map<int, char> m_IntCharMap;
};
}