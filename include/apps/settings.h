#pragma once

#include "app.h"

class Settings : public App {
public:
    Settings(TTGOClass* ttgo)
        : App("Settings", ttgo)
    {}

    virtual void update() override;
    virtual void render() override;
};