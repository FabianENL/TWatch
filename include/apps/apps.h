#pragma once

#include <vector>

#include "app.h"
#include "event/inputevent.h"

class Apps : public App {
public:
    static std::vector<App*> apps;

    Apps(TTGOClass* ttgo)
        : App("Apps", ttgo)
    {}

    virtual void onEvent(Event& event) override;
    virtual void update() override;
    virtual void render() override;
private:
    bool onMoveEvent(MoveEvent& event);
private:
    int16_t m_scrollY;
};
