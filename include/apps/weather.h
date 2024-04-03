#pragma once

#include "app.h"

class Weather : public App {
public:
    Weather()
        : App("Weather")
    {}

    virtual void update() override;
    virtual void render() override;
};