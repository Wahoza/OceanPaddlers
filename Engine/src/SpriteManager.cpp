//Author: Radomir Dinic
//Description: Singelton, manages Sprites
//License: LICENSE.txt
#include "stdafx.h"

#include "SpriteManager.hpp"

#include "GameObject.hpp"
#include "IRenderComponent.hpp"
#include "RenderComponentEvents.hpp"

namespace mmt_gd
{
void SpriteManager::init()
{
    // subscribe to creation events
    {
        const auto id = EventBus::getInstance()
                            .addListener(RenderableCreateEvent::Type,
                                         [this](const IEvent::Ptr& event)
                                         {
                                             const auto renderableCreateEvent = std::static_pointer_cast<RenderableCreateEvent>(
                                                 event);
                                             this->addCompToLayer(renderableCreateEvent->getLayer(),
                                                                  &renderableCreateEvent->getData());
                                         });
        m_listeners.push_back(id);
    }
    {
        const auto id = EventBus::getInstance()
                            .addListener(RenderableDestroyEvent::Type,
                                         [this](const IEvent::Ptr& event)
                                         {
                                             const auto destroyEvent = std::static_pointer_cast<RenderableDestroyEvent>(
                                                 event);
                                             this->removeComp(&destroyEvent->getData());
                                         });
        m_listeners.push_back(id);
    }
}

void SpriteManager::shutdown()
{
    m_layerOrder.clear();
    m_layerToComp.clear();
    m_compToLayer.clear();
    m_tileSets.clear();
    m_camera = nullptr;

    // unsubscribe from events
    for (const auto& listener : m_listeners)
    {
        EventBus::getInstance().removeListener(listener);
    }
    m_listeners.clear();
}

void SpriteManager::addCompToLayer(const std::string& layer, IRenderComponent* comp)
{
    if (m_compToLayer.find(comp) != m_compToLayer.end())
    {
        ffErrorMsg("Removing component from layer, before adding to new layer. from " + m_compToLayer[comp] + " to " + layer)

            m_layerToComp[layer]
                .remove(comp);
    }

    m_compToLayer[comp] = layer;
    m_layerToComp[layer].push_back(comp);
}

void SpriteManager::removeComp(IRenderComponent* comp)
{
    const auto it = m_compToLayer.find(comp);
    if (it == m_compToLayer.end())
    {
        return;
    }

    m_layerToComp[it->second].remove(comp);
    m_compToLayer.erase(it);
}

std::string SpriteManager::getLayer(const IRenderComponent* comp)
{
    if (const auto it = m_compToLayer.find(comp); it != m_compToLayer.end())
    {
        return it->second;
    }
    return {};
}

void SpriteManager::draw()
{
    ffAssertMsg(m_camera != nullptr, "Camera is nullptr")

        m_camera->draw();


    const auto& order = m_layerOrder[3];

    PROFILE_SCOPE(order);

    ffAssertMsg(m_layerToComp.find(order) != m_layerToComp.end(), "Layer with this name not defined: " + order)

        auto& layer = m_layerToComp[order];
    for (auto* comp : layer)
    {
        if (comp->getGameObject().isActive())
        {
            comp->draw();
        }
    }
}
void SpriteManager::drawBackground()
{
    //{"Floor", "Background", "Objects", "GameObjects", "Top"}
    ffAssertMsg(m_camera != nullptr, "Camera is nullptr")

        m_camera->draw();


    const auto& order = m_layerOrder[1];

    PROFILE_SCOPE(order);

    ffAssertMsg(m_layerToComp.find(order) != m_layerToComp.end(), "Layer with this name not defined: " + order)

        auto& layer = m_layerToComp[order];
    for (auto* comp : layer)
    {
        if (comp->getGameObject().isActive())
        {
            comp->draw();
        }
    }
}
void SpriteManager::drawTop()
{
    ffAssertMsg(m_camera != nullptr, "Camera is nullptr")

        m_camera->draw();


    const auto& order = m_layerOrder[4];

    PROFILE_SCOPE(order);

    ffAssertMsg(m_layerToComp.find(order) != m_layerToComp.end(), "Layer with this name not defined: " + order)

        auto& layer = m_layerToComp[order];
    for (auto* comp : layer)
    {
        if (comp->getGameObject().isActive())
        {
            comp->draw();
        }
    }
}
} // namespace mmt_gd
