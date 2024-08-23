#pragma once
#include "stdafx.h"
#include "TriggerComponent.hpp"

namespace mmt_gd
{
class TransitionObject
{
public:
    void init(sf::Vector2i initRect, int initOffset);
    void update(float deltatime, sf::FloatRect Area);
    void draw(sf::RenderWindow& window);

    sf::FloatRect* getDisabledArea();
    void          Notify();
    void genTransitionPos();
    void shutdown();
    Vector2f getPosition();

private:
    sf::Sprite transitionSprite;
    sf::Texture transitionTexture;

    int   TextureSize    = 16;
    float TransitionDist = 2 * TextureSize;

    float transitionTime = 3;
    float             areaWidth      = 30;

    bool              transitioning  = false;
    float             elapsedTransTime    = 0;
    float             maxTime          = 0.5;

    TriggerComponent* trigger = nullptr;
    sf::FloatRect     _windowRect = {0, 0, 0, 0};
    sf::FloatRect     disabledArea   = {0, 0, 0, 0};
    sf::Vector2i     _area        = {0, 0};
    bool              isInit         = false;
};
}