#include "apps/apps.h"

#include "apps/settings.h"
#include "apps/tools.h"
#include "apps/weather.h"

std::vector<App*> Apps::apps = { new Apps(), new Settings(), new Tools(), new Weather() };
App* Apps::activeApp = nullptr;

void Apps::init()
{
    m_list.init(getParent());
    m_list.setCallback(BIND_FN(Apps::listCallback));
    for (size_t i = 1; i < apps.size(); ++i) {
        m_list.add(apps[i]->getName());
    }
}

void Apps::listCallback(String txt)
{
    Serial.printf("Opening app: %s\n", txt.c_str());
    for (size_t i = 1; i < apps.size(); ++i) {
        App* app = apps[i];
        if (txt.compareTo(app->getName()) == 0) {
            lv_obj_set_hidden(activeApp->getParent(), true);
            lv_obj_set_hidden(app->getParent(), false);
            activeApp = app;
        }
    }
}