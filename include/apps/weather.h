#pragma once

#include "app.h"

class Weather : public App {
public:
    Weather(TTGOClass* ttgo)
        : App("Weather", ttgo)
    {}

    virtual void update() override;
    virtual void render() override;
};