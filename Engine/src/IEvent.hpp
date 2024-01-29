#pragma once

#include <memory>

namespace mmt_gd
{
class IEvent
{
public:
    IEvent()                                 = default;
    virtual ~IEvent()                        = default;
    IEvent(const IEvent& other)              = delete;
    IEvent(const IEvent&& other)             = delete;
    IEvent&  operator=(const IEvent& other)  = delete;
    IEvent&& operator=(const IEvent&& other) = delete;

    using EventType = unsigned long;
    using Ptr       = std::shared_ptr<IEvent>;

    virtual EventType getEventType() = 0;
};

template <typename T>

class BasicDataEvent : public IEvent
{
public:
    explicit BasicDataEvent(T data) : m_data(std::move(data))
    {
    }

    ~BasicDataEvent() override                               = default;
    BasicDataEvent(const BasicDataEvent& other)              = delete;
    BasicDataEvent(const BasicDataEvent&& other)             = delete;
    BasicDataEvent&  operator=(const BasicDataEvent& other)  = delete;
    BasicDataEvent&& operator=(const BasicDataEvent&& other) = delete;

    T getData()
    {
        return m_data;
    }

private:
    T m_data;
};

} // namespace mmt_gd
