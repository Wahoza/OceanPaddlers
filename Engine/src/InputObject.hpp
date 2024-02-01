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

    void draw();


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

};
}