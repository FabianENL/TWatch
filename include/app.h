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

    // Gets called at startup after the ttgo watch has been initialized
    virtual void init() { }

    // Gets called every time the app opens
    virtual void onOpen() { }

    // Gets called every time the app closes
    virtual void onClose() { }

    // Gets called when any event happens
    virtual void onEvent(Event& event) { }

    // Gets called every frame
    virtual void update() { }

    String getName() const { return m_name; }
    lv_obj_t* getParent() const { return m_parent; }
private:
    String m_name;
    lv_obj_t* m_parent;
};
