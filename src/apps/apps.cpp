#include "apps/apps.h"

#include "apps/settings.h"
#include "apps/tools.h"
#include "apps/weather.h"

std::vector<App*> Apps::apps = {new Apps(TTGOClass::getWatch()),
                                new Tools(TTGOClass::getWatch()),
                                new Settings(TTGOClass::getWatch()), 
                                new Weather(TTGOClass::getWatch())}; 

void Apps::onEvent(Event& event) {
    EventDispatcher dispatcher(event);
    dispatcher.dispatch<MoveEvent>(BIND_FN(Apps::onMoveEvent));
}

bool Apps::onMoveEvent(MoveEvent& event) {
    auto [dx, dy] = event.getDeltaPos();
    m_scrollY += dy;
    return false;
}

void Apps::update() {
    
}

void Apps::render() {
    int textHeight = m_ttgo->tft->fontHeight(4);
    int height = textHeight * 3;

    // Iterate over all the apps except the first one as it's the "apps" app
    for (int i = 1; i < apps.size(); i++) {
        int y = m_scrollY + i * height;
        if (y > 0 && y < 240 - textHeight) {
            m_ttgo->tft->drawCentreString(apps[i]->getName(), 120, y, 4);
        }
    }
}
