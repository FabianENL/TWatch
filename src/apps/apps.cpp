#include "apps/apps.h"

#include "apps/settings.h"
#include "apps/tools.h"
#include "apps/weather.h"

std::vector<App*> Apps::apps = { new Apps(), new Tools(), new Settings(), new Weather() };

void Apps::init()
{
    m_list.init(lv_scr_act());
    m_list.align(nullptr, LV_ALIGN_CENTER);
    m_list.setListCallback(BIND_FN(Apps::listCallback));
    for (App* app : apps) {
        m_list.add(app->getName());
    }
    registerLvObj(m_list.getObj());
}

void Apps::onEvent(Event& event)
{
    EventDispatcher dispatcher(event);
    dispatcher.dispatch<MoveEvent>(BIND_FN(Apps::onMoveEvent));
}

bool Apps::onMoveEvent(MoveEvent& event)
{
    auto [dx, dy] = event.getDeltaPos();
    m_scrollY += dy;
    return false;
}

void Apps::update()
{
    // int textHeight = m_ttgo->tft->fontHeight(4);
    // int height = textHeight * 3;

    // // Iterate over all the apps except the first one as it's the "apps" app
    // for (int i = 1; i < apps.size(); i++) {
    //     int y = m_scrollY + i * height;
    //     if (y > 0 && y < 240 - textHeight) {
    //         m_ttgo->tft->drawCentreString(apps[i]->getName(), 120, y, 4);
    //     }
    // }
}

void Apps::listCallback(String txt)
{
}