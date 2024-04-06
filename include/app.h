#pragma once

#include "core.h"
#include "event/event.h"

#include <vector>

class App {
public:
    App(const String& name)
        : m_name(name)
    {
    }

    void preinit(lv_obj_t* parent) { m_parent = parent; }
    virtual void init() { }
    virtual void onEvent(Event& event) { }
    virtual void update() { }

    String getName() const { return m_name; }
    lv_obj_t* getParent() const { return m_parent; }
private:
    String m_name;
    lv_obj_t* m_parent;
};
