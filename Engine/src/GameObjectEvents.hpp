//Author: Radomir Dinic, modified by Kilian Kolb
//Description: All Event types in the Game.
//License: LICENSE.txt
#pragma once

#include "GameObject.hpp"
#include "IEvent.hpp"

namespace mmt_gd
{
class GameObjectCreateEvent final : public BasicDataEvent<GameObject::Ptr>
{
public:
    using BasicDataEvent::BasicDataEvent;

    /**
     * \brief m_eventType defined using GUID functionality of VS 2015.
     * Tools->Create GUID -> DEFINE_GUID -> take longest value in the beginning.
     * output:
     *
     * {DEC71435-8D92-4CA7-A3D5-FD17F9D5CC34}
     * DEFINE_GUID(<< name >> ,
     * 0xdec71435, 0x8d92, 0x4ca7, 0xa3, 0xd5, 0xfd, 0x17, 0xf9, 0xd5, 0xcc, 0x34);
     *
     * m_EventType identifies event, when we want to add it to the event bus
     */
    static const EventType Type;

    EventType getEventType() override
    {
        return Type;
    }
};

class GameObjectDestroyEvent final : public BasicDataEvent<GameObject::Ptr>
{
public:
    using BasicDataEvent::BasicDataEvent;

    static const EventType Type;

    EventType getEventType() override
    {
        return Type;
    }
};


//GameObject Score Events:

class ScoreEvent : public BasicDataEvent<std::pair<int, std::string>>
{
public:
    using BasicDataEvent::BasicDataEvent;

    static const EventType Type;

    EventType getEventType() override
    {
        return Type;
    }
};

class EggEvent final : public BasicDataEvent<int>
{
public:
    using BasicDataEvent::BasicDataEvent;

    static const EventType Type;

    EventType getEventType() override
    {
        return Type;
    }
};

class HurtEvent final : public BasicDataEvent<int>
{
public:
    using BasicDataEvent::BasicDataEvent;

    static const EventType Type;

    EventType getEventType() override
    {
        return Type;
    }
};

//GameOverEvent: Distance Traveled, Eggs Collected, Fish Killed, reached end
class GameOverEvent final : public BasicDataEvent<std::tuple<float, int, int, bool, int>>
{
public:
    using BasicDataEvent::BasicDataEvent;

    static const EventType Type;

    EventType getEventType() override
    {
        return Type;
    }
};
class FishCollisionEvent final : public BasicDataEvent<std::tuple<bool>>
{
public:
    using BasicDataEvent::BasicDataEvent;

    static const EventType Type;

    EventType getEventType() override
    {
        return Type;
    }
};
class IslandCollisionEvent final : public BasicDataEvent<sf::FloatRect>
{
public:
    using BasicDataEvent::BasicDataEvent;

    static const EventType Type;

    EventType getEventType() override
    {
        return Type;
    }
};
class InputEvent final : public BasicDataEvent<char>
{
public:
    using BasicDataEvent::BasicDataEvent;

    static const EventType Type;

    EventType getEventType() override
    {
        return Type;
    }
};
class VictorySoundEvent final : public BasicDataEvent<bool>
{
public:
    using BasicDataEvent::BasicDataEvent;

    static const EventType Type;

    EventType getEventType() override
    {
        return Type;
    }
};
class WaveSoundEvent final : public BasicDataEvent<bool>
{
public:
    using BasicDataEvent::BasicDataEvent;

    static const EventType Type;

    EventType getEventType() override
    {
        return Type;
    }
};
class IslandTileGenEvent final : public BasicDataEvent <std::vector<int>>
{
public:
    using BasicDataEvent::BasicDataEvent;

    static const EventType Type;

    EventType getEventType() override
    {
        return Type;
    }
};
class SwapTiles final : public BasicDataEvent<std::tuple<int, int>>
{
public:
    using BasicDataEvent::BasicDataEvent;

    static const EventType Type;

    EventType getEventType() override
    {
        return Type;
    }
};
class SwapTileRequest final : public BasicDataEvent<bool>
{
public:
    using BasicDataEvent::BasicDataEvent;

    static const EventType Type;

    EventType getEventType() override
    {
        return Type;
    }
};
class PauseEvent final : public BasicDataEvent<bool>
{
public:
    using BasicDataEvent::BasicDataEvent;

    static const EventType Type;

    EventType getEventType() override
    {
        return Type;
    }
};
class AttackEvent final : public BasicDataEvent<bool>
{
public:
    using BasicDataEvent::BasicDataEvent;

    static const EventType Type;

    EventType getEventType() override
    {
        return Type;
    }
};
class FishHitSuccess final : public BasicDataEvent<bool>
{
public:
    using BasicDataEvent::BasicDataEvent;

    static const EventType Type;

    EventType getEventType() override
    {
        return Type;
    }
};
class HealthCollect final : public BasicDataEvent<int>
{
public:
    using BasicDataEvent::BasicDataEvent;

    static const EventType Type;

    EventType getEventType() override
    {
        return Type;
    }
};
class PowerUP final : public BasicDataEvent<bool>
{
public:
    using BasicDataEvent::BasicDataEvent;

    static const EventType Type;

    EventType getEventType() override
    {
        return Type;
    }
};
class FishKill final : public BasicDataEvent<int>
{
public:
    using BasicDataEvent::BasicDataEvent;

    static const EventType Type;

    EventType getEventType() override
    {
        return Type;
    }
};

} // namespace mmt_gd
