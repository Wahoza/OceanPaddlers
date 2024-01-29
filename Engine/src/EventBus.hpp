#pragma once

#include "IEvent.hpp"

#include <functional>
#include <list>
#include <queue>
#include <unordered_map>

namespace mmt_gd
{
/**
 * \brief rudimentary event bus (implemented as Singleton)
 * for registering delegates that execute code, when event is triggered.
 * add ons for the future: abort events, timer to limit processing of
 * events, ...
 *
 * BE CAREFUL: the EventBus has not been thoroughly tested.
 */
class EventBus
{
public:
    using EventListener = std::function<void(IEvent::Ptr)>;
    using ListenerId    = unsigned long;

    static EventBus& getInstance();

    /**
     * \brief´Add/remove event listeners
     * Be careful:
     * Due to the usage of std::function, multiple additions of the
     * same function to the m_listener cannot be checked.
     * \param type Type of the event
     * \param listener event listener
     * \return ID of the Listener
     */
    ListenerId addListener(IEvent::EventType type, EventListener listener);
    void       removeListener(ListenerId id);

    /**
     * \brief immediately triggers the event
     * \param event event to be fired
     */
    void fireEvent(const IEvent::Ptr& event);

    /**
     * \brief queue event to be processed
     * \param event event to be queued
     */
    void queueEvent(const IEvent::Ptr& event);

    /**
     * \brief should called once per frame to process events
     */
    void processEvents(float deltaTime);

private:
    struct ListenerData
    {
        ListenerId    m_id;
        EventListener m_listener;
    };

    using ListenerList       = std::list<ListenerData>;
    using EventListenerMap   = std::unordered_map<IEvent::EventType, ListenerList>;
    using ListenerIdEventMap = std::unordered_map<ListenerId, IEvent::EventType>;
    using EventQueue         = std::queue<IEvent::Ptr>;

    static constexpr int NumMaxQueues = 2;
    static ListenerId    m_listenerIdCtr;

    int m_currentEventQueue = 0;

    /**
     * \brief buffering the events to avoid circles when new events are
     * queued each frame before the old ones are processed.
     */
    EventQueue m_eventQueueBuffer[NumMaxQueues];

    /**
     * \brief The map stores the registered listeners for each event.
     */
    EventListenerMap m_eventListenerMap;

    /**
     * \brief This map is used for faster lookups when removing listeners.
     */
    ListenerIdEventMap m_listenerIdEventMap;
};
} // namespace mmt_gd
