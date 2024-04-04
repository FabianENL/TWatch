#pragma once

#define LILYGO_WATCH_2020_V3
#include <LilyGoWatch.h>
#include <Arduino.h>

#include "event/event.h"

class App {
public:
    App(const String& name, TTGOClass* ttgo)
        : m_name(name), m_ttgo(ttgo) {}

    virtual void init() {}
    virtual void onEvent(Event& event) {}
    virtual void update() {}
    virtual void render() {}

    String getName() const { return m_name; }
protected:
    TTGOClass* m_ttgo;
private:
    String m_name;
};
