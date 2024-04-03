#pragma once

#include "event/event.h"

#include <string>

#define BIND_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

class App {
public:
    App(const String& name)
        : m_name(name) {}

    virtual void onEvent(Event& event) {}
    virtual void update() {}
    virtual void render() {}

    String getName() const { return m_name; }
private:
    String m_name;
};
