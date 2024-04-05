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

    virtual void init() { }
    virtual void onEvent(Event& event) { }
    virtual void update() { }

    void registerLvObj(lv_obj_t* obj)
    {
        m_objects.push_back(obj);
    }

    String getName() const { return m_name; }
    const std::vector<lv_obj_t*>& getObjects() const { return m_objects; }
private:
    String m_name;
    std::vector<lv_obj_t*> m_objects;
};
