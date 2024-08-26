#include "stdafx.h"
#include "TransitionObject.hpp"
#include "EventBus.hpp"
#include "GameObjectEvents.hpp"
#include "SoundManager.hpp"
#include "PhysicsManager.hpp"
namespace mmt_gd
{
void TransitionObject::init(sf::Vector2i Area, int initOffset)
{
    _windowRect = {0, 0, (float)Area.x * TextureSize, (float)Area.y * TextureSize};
    _area = Area * TextureSize;


    trigger = new TriggerComponent({0, 0, 0, 0}, "Player0", "Transition");

    PhysicsManager::getInstance()->RegisterTrigger(trigger);
    trigger->setEnabled(true);

    disabledArea = {_windowRect.left + TransitionDist + initOffset * TextureSize, 0, (float)TextureSize * areaWidth, _windowRect.height};
    trigger->m_shape = {_windowRect.left + TransitionDist + initOffset * TextureSize, 0, (float)TextureSize, _windowRect.height};

    if (!transitionTexture.loadFromFile("../assets/SegmentedTiles/transitionTile.png"))
    {
        sf::err() << "Could not load swapper texture\n";
    }
    transitionSprite.setTexture(transitionTexture);
    transitionSprite.setPosition(trigger->getPosition());

    if (isInit)
    {
        return;
    }
    isInit = true;
    EventBus::getInstance().addListener(SwapTileRequest::Type, [this](IEvent::Ptr event) { Notify(); });
    //genTransitionPos();
    //std::cout << "I exist at " + std::to_string(trigger->getPosition().x) << std::endl;
}
void TransitionObject::update(float deltaTime, sf::FloatRect windowRect)
{
    _windowRect = windowRect;
    if (transitioning)
    {
        elapsedTransTime += deltaTime;

        if (elapsedTransTime > maxTime)
        {
            elapsedTransTime = 0;
            transitioning    = false;
            transitionSprite.setPosition(trigger->getPosition());
        }
    }
}
void TransitionObject::draw(sf::RenderWindow& window)
{
    window.draw(transitionSprite);
}

sf::FloatRect* TransitionObject::getDisabledArea()
{
    return &disabledArea;
}

void TransitionObject::Notify()
{
    genTransitionPos();
    std::cout << trigger->m_shape.left << std::endl;
    trigger->m_identifier = "Player0";

    //SoundManager::getInstance()->playSound("Victory");
}

void TransitionObject::genTransitionPos()
{
    float xpos = (_windowRect.left + TransitionDist + _windowRect.width) -
                 (int(_windowRect.left + TransitionDist + _windowRect.width) % TextureSize) + _area.x;

    trigger->m_shape = {xpos, 0, (float)TextureSize, _windowRect.height};
    disabledArea = {xpos - 2 * TextureSize, 0, (float)TextureSize * areaWidth, _windowRect.height};
    transitioning    = true;
}
void TransitionObject::shutdown()
{
}
Vector2f TransitionObject::getPosition()
{
    if (trigger == nullptr)
    {
        return {0, 0};
    }

    return trigger->getPosition();
}
} // namespace mmt_gd