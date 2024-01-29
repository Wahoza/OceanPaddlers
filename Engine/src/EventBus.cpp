#include "stdafx.h"

#include "EventBus.hpp"

#include <utility>

namespace mmt_gd
{
EventBus::ListenerId EventBus::m_listenerIdCtr = 0;

EventBus& EventBus::getInstance()
{
    static EventBus instance;
    return instance;
}

EventBus::ListenerId EventBus::addListener(const IEvent::EventType type, EventListener listener)
{
    ++m_listenerIdCtr;
    m_eventListenerMap[type].push_back({m_listenerIdCtr, std::move(listener)});
    m_listenerIdEventMap[m_listenerIdCtr] = type;
    return m_listenerIdCtr;
}

void EventBus::removeListener(const ListenerId id)
{
    const auto eventTypeIt = m_listenerIdEventMap.find(id);
    if (eventTypeIt == m_listenerIdEventMap.end())
    {
        return;
    }

    const auto eventListenerIt = m_eventListenerMap.find(eventTypeIt->second);
    if (eventListenerIt == m_eventListenerMap.end())
    {
        ffErrorMsg("Possible error: did not find event type in m_listener list.") return;
    }

    auto  found        = false; ///< for debugging
    auto& listenerList = eventListenerIt->second;
    for (auto it = listenerList.begin(); it != listenerList.end(); ++it)
    {
        if ((*it).m_id != id)
        {
            continue;
        }
        it = listenerList.erase(it);
        m_listenerIdEventMap.erase(id);
        found = true;
        break;
    }

    if (found)
    {
        sf::err() << "Removed m_listener with given m_id. " << id << std::endl;
    }
    else
    {
        sf::err() << "Possible error: did not find m_listener with this m_id for this event. " << id << " "
                  << eventTypeIt->second << std::endl;
    }
}

void EventBus::fireEvent(const IEvent::Ptr& event)
{
    const auto it = m_eventListenerMap.find(event->getEventType());
    if (it == m_eventListenerMap.end())
    {
        return;
    }

    const auto& listener_list = it->second;
    for (const auto& listenerData : listener_list)
    {
        listenerData.m_listener(event);
    }
}

void EventBus::queueEvent(const IEvent::Ptr& event)
{
    m_eventQueueBuffer[m_currentEventQueue].push(event);
}

void EventBus::processEvents(float deltaTime)
{
    const auto currentQueue = m_currentEventQueue;
    while (!m_eventQueueBuffer[currentQueue].empty())
    {
        auto event = m_eventQueueBuffer[currentQueue].front();
        m_eventQueueBuffer[currentQueue].pop();

        fireEvent(event);
    }

    // switch buffer
    m_currentEventQueue = (currentQueue + 1) % NumMaxQueues;
}
} // namespace mmt_gd
