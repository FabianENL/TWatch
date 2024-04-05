#pragma once

#include "app.h"

class Settings : public App {
public:
    Settings()
        : App("Settings")
    {
    }

    virtual void update() override;
};