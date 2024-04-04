#pragma once

#include "app.h"

class Tools : public App {
public:
    Tools(TTGOClass* ttgo)
        : App("Tools", ttgo)
    {}

    virtual void update() override;
    virtual void render() override;
};