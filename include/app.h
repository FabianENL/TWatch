#pragma once

#include <string>

class App {
public:
    App(const String& name)
        : m_name(name) {}

    virtual void update() = 0;
    virtual void render() = 0;

    String getName() const { return m_name; }
private:
    String m_name;
};
