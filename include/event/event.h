#pragma once

#include "core.h"

#include <functional>

enum class EventType {
    None = 0,
    Press,
    Release,
    Click,
    Swipe,
    Move
};

enum EventCategory {
    None = 0,
    EventCategoryInput = BIT(0),
};

#define EVENT_CLASS_TYPE(type)                                                  \
    static EventType getStaticType() { return EventType::type; }                \
    virtual EventType getEventType() const override { return getStaticType(); } \
    virtual String getName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) \
    virtual int getCategoryFlags() const override { return category; }

class Event {
public:
    virtual ~Event() = default;

    bool handled = false;

    virtual EventType getEventType() const = 0;
    virtual String getName() const = 0;
    virtual int getCategoryFlags() const = 0;

    inline bool isInCategory(EventCategory category) { return getCategoryFlags() & category; }
};

class EventDispatcher {
    template <typename T>
    using EventFn = std::function<bool(T&)>;

public:
    EventDispatcher(Event& event)
        : m_event(event)
    {
    }

    template <typename T>
    bool dispatch(EventFn<T> func)
    {
        if (m_event.getEventType() == T::getStaticType()) {
            m_event.handled = func(static_cast<T&>(m_event));
            return true;
        }
        return false;
    }

private:
    Event& m_event;
};
