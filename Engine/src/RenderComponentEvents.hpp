#pragma once

#include "IEvent.hpp"
#include "SpriteRenderComponent.hpp"
#include "TileLayerRenderComponent.hpp"

#include <utility>

namespace mmt_gd
{
class RenderableCreateEvent final : public BasicDataEvent<IRenderComponent&>
{
public:
    RenderableCreateEvent(std::string layer, IRenderComponent& comp) : BasicDataEvent(comp), m_layer(std::move(layer))
    {
    }

    static const EventType Type;

    EventType getEventType() override
    {
        return Type;
    }

    std::string getLayer() const
    {
        return m_layer;
    }

private:
    std::string m_layer;
};

class RenderableDestroyEvent final : public BasicDataEvent<IRenderComponent&>
{
public:
    RenderableDestroyEvent(std::string layer, IRenderComponent& comp) : BasicDataEvent(comp), m_layer(std::move(layer))
    {
    }

    static const EventType Type;

    EventType getEventType() override
    {
        return Type;
    }

    std::string getLayer() const
    {
        return m_layer;
    }

private:
    std::string m_layer;
};
} // namespace mmt_gd
