#pragma once
#include "stdafx.h"

#include "IComponent.hpp"
#include "IRenderComponent.hpp"

#include "GameObject.hpp"

using namespace sf;
using namespace std;
namespace mmt_gd
{
class ScoreDisplayComponent : public IRenderComponent
{
public:
    ScoreDisplayComponent(GameObject& gameObject, RenderWindow& renderWindow, string _texts);
    bool init() override;
    void update(float deltaTime) override;
    void draw() override;
    ~ScoreDisplayComponent();
    Text m_text;
    Font m_font;

    Vector2f m_position;

private:
};
}