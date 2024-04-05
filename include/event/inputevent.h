#pragma once

#include "event.h"

#include <string>

class MoveEvent : public Event {
public:
    MoveEvent(int16_t x, int16_t y, int16_t startX, int16_t startY)
        : m_x(x)
        , m_y(y)
    {
    }

    inline int16_t getX() const { return m_x; }
    inline int16_t getY() const { return m_y; }
    inline std::pair<int16_t, int16_t> getPos() const { return std::make_pair(m_x, m_y); }

    inline int16_t getStartX() const { return m_startX; }
    inline int16_t getStartY() const { return m_startY; }
    inline std::pair<int16_t, int16_t> getStartPos() const { return std::make_pair(m_startX, m_startY); }

    inline int16_t getDeltaX() const { return m_x - m_startX; }
    inline int16_t getDeltaY() const { return m_y - m_startY; }
    inline std::pair<int16_t, int16_t> getDeltaPos() const { return std::make_pair(m_x - m_startX, m_x - m_startY); }

    EVENT_CLASS_TYPE(Move)
    EVENT_CLASS_CATEGORY(EventCategoryInput)
private:
    int16_t m_x, m_y;
    int16_t m_startX, m_startY;
};

class PressEvent : public Event {
public:
    PressEvent(int16_t x, int16_t y)
        : m_x(x)
        , m_y(y)
    {
    }

    inline int16_t getX() const { return m_x; }
    inline int16_t getY() const { return m_y; }
    inline std::pair<int16_t, int16_t> getPos() const { return std::make_pair(m_x, m_y); }

    EVENT_CLASS_TYPE(Press)
    EVENT_CLASS_CATEGORY(EventCategoryInput)
private:
    int16_t m_x, m_y;
};

class ReleaseEvent : public Event {
public:
    ReleaseEvent(int16_t x, int16_t y, int16_t startX, int16_t startY)
        : m_x(x)
        , m_y(y)
        , m_startX(startX)
        , m_startY(startY)
    {
    }

    inline int16_t getX() const { return m_x; }
    inline int16_t getY() const { return m_y; }
    inline std::pair<int16_t, int16_t> getPos() const { return std::make_pair(m_x, m_x); }

    inline int16_t getStartX() const { return m_startX; }
    inline int16_t getStartY() const { return m_startY; }
    inline std::pair<int16_t, int16_t> getStartPos() const { return std::make_pair(m_startX, m_startY); }

    inline int16_t getDeltaX() const { return m_x - m_startX; }
    inline int16_t getDeltaY() const { return m_y - m_startY; }
    inline std::pair<int16_t, int16_t> getDeltaPos() const { return std::make_pair(m_x - m_startX, m_x - m_startY); }

    EVENT_CLASS_TYPE(Release)
    EVENT_CLASS_CATEGORY(EventCategoryInput)
private:
    int16_t m_x, m_y;
    int16_t m_startX, m_startY;
};

class ClickEvent : public Event {
public:
    ClickEvent(int16_t x, int16_t y)
        : m_x(x)
        , m_y(y)
    {
    }

    inline int16_t getX() const { return m_x; }
    inline int16_t getY() const { return m_y; }
    inline std::pair<int16_t, int16_t> getPos() const { return std::make_pair(m_x, m_x); }

    EVENT_CLASS_TYPE(Click)
    EVENT_CLASS_CATEGORY(EventCategoryInput)
private:
    int16_t m_x, m_y;
};

enum class SwipeDirection {
    None = 0,
    Left,
    Right,
    Up,
    Down
};

class SwipeEvent : public Event {
public:
    SwipeEvent(int16_t x, int16_t y, int16_t startX, int16_t startY)
        : m_x(x)
        , m_y(y)
        , m_startX(startX)
        , m_startY(startY)
    {
    }

    inline int16_t getX() const { return m_x; }
    inline int16_t getY() const { return m_y; }
    inline std::pair<int16_t, int16_t> getPos() const { return std::make_pair(m_x, m_x); }

    inline int16_t getStartX() const { return m_startX; }
    inline int16_t getStartY() const { return m_startY; }
    inline std::pair<int16_t, int16_t> getStartPos() const { return std::make_pair(m_startX, m_startY); }

    inline int16_t getDeltaX() const { return m_x - m_startX; }
    inline int16_t getDeltaY() const { return m_y - m_startY; }
    inline std::pair<int16_t, int16_t> getDeltaPos() const { return std::make_pair(m_x - m_startX, m_x - m_startY); }

    inline SwipeDirection getSwipeDirection() const
    {
        auto [dx, dy] = getDeltaPos();
        if (abs(dx) > abs(dy)) {
            if (dx > 0)
                return SwipeDirection::Right;
            else if (dx < 0)
                return SwipeDirection::Left;
        } else {
            if (dy > 0)
                return SwipeDirection::Down;
            else if (dy < 0)
                return SwipeDirection::Up;
        }

        return SwipeDirection::None;
    }

    EVENT_CLASS_TYPE(Swipe)
    EVENT_CLASS_CATEGORY(EventCategoryInput)
private:
    int16_t m_x, m_y;
    int16_t m_startX, m_startY;
};
