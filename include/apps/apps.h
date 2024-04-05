#pragma once

#include "core.h"

#include <vector>

#include "app.h"
#include "elements/list.h"
#include "event/inputevent.h"

class Apps : public App {
public:
    static std::vector<App*> apps;

    Apps()
        : App("Apps")
    {
    }

    virtual void init() override;
    virtual void onEvent(Event& event) override;
    virtual void update() override;

private:
    bool onMoveEvent(MoveEvent& event);
    void listCallback(String txt);

private:
    int16_t m_scrollY;
    List m_list;
};
