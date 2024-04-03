#pragma once

#include <string>

class App {
public:
    App(const std::string& name)
        : m_name(name) {}

    virtual void update() = 0;
    virtual void render() = 0;
private:
    std::string m_name;
};
