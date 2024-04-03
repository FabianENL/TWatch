#pragma once

#include "app.h"

class Tools : public App {
public:
    Tools()
        : App("Tools")
    {}

    virtual void update() override;
    virtual void render() override;
};