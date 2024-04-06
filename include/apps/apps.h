#pragma once

#include "core.h"

#include <vector>

#include "app.h"
#include "elements/list.h"
#include "event/inputevent.h"

class Apps : public App {
public:
    static std::vector<App*> apps;
    static App* activeApp;

    Apps()
        : App("Apps")
    {
    }

    virtual void init() override;

private:
    void callback(String txt);

private:
    int16_t m_scrollY;
    List m_list;
};
