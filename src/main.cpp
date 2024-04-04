#define LILYGO_WATCH_2020_V3
#define LILYGO_WATCH_LVGL 
#include <LilyGoWatch.h>

#include <vector>

#include "app.h"
#include "apps/apps.h"
#include "apps/settings.h"
#include "apps/tools.h"
#include "apps/weather.h"
#include "event/event.h"
#include "event/inputevent.h"

enum class Menu {
    None = 0,
    Home,
    App
};

TTGOClass *ttgo;
int updateTimer = 0;
int sleepTimer = 0;
bool irq = false;
int16_t scrollY;
int16_t prevX, prevY;
Menu menu = Menu::Home;

bool pressing = false;
bool prevPressing = false;
int16_t startX, startY;

App* activeApp = nullptr;

void drawTime() {
    ttgo->tft->setTextColor(TFT_WHITE, TFT_BLACK);
    ttgo->tft->setTextSize(2);
    ttgo->tft->setCursor(0, 0);
    ttgo->tft->print(ttgo->power->getBattPercentage());
    ttgo->tft->print("%");
    ttgo->tft->setTextSize(1);
    ttgo->tft->drawCentreString(ttgo->rtc->formatDateTime(PCF_TIMEFORMAT_HM),
                                120, 60, 7);
}

void setup() {
    Serial.begin(115200);

    ttgo = TTGOClass::getWatch();
    ttgo->begin();              // Initialize the ttgo library
    ttgo->motor_begin();        // Initialize the vibration motor
    ttgo->openBL();             // Enable the backlight
    ttgo->setBrightness(128);   // Set the backlight, 0-255    
    ttgo->lvgl_begin();         // Initialize the lvgl library

    // Enable the power button
    pinMode(AXP202_INT, INPUT_PULLUP);
    attachInterrupt(
        AXP202_INT, [] { irq = true; }, FALLING);
    ttgo->power->enableIRQ(AXP202_PEK_SHORTPRESS_IRQ, true);
    ttgo->power->clearIRQ();

    drawTime();

    // Initialize all apps after the ttgo library and it's components have been initialized
    for (App* app : Apps::apps) {
        app->init();
    }
}

void loop() {
    ttgo->power->clearIRQ();

    int16_t x, y;
    pressing = ttgo->getTouch(x, y);
    if (pressing) {
        if (!prevPressing) {
            // On Press event
            if (activeApp) {
                PressEvent e(x, y);
                activeApp->onEvent(e);
            }
            startX = x;
            startY = y;
        }

        if (x != prevX || y != prevY) {
            // On Move Event
            if (activeApp) {
                MoveEvent e(x, y, startX, startY);
                activeApp->onEvent(e);
            }
        }

        sleepTimer = ttgo->rtc->getDateTime().second;
        if (menu == Menu::Home) {
            activeApp = Apps::apps[0];
            menu = Menu::App;
            ttgo->tft->fillScreen(TFT_BLACK);
        }

        prevX = x;
        prevY = y;
    } else {
        if (prevPressing) {
            // On Release Event
            if (activeApp) {
                ReleaseEvent e(x, y, startX, startY);
                activeApp->onEvent(e);
            }

            int16_t dx = x - startX;
            int16_t dy = y - startY;
            // If the change of finger position is less than x amount pixels, then its a click
            if (abs(dx) < 10 && abs(dy) < 10) {
                // On Click Event
                if (activeApp) {
                    ClickEvent e(x, y);
                    activeApp->onEvent(e);
                }
            } else {
                // On Swipe Event
                if (activeApp) {
                    SwipeEvent e(x, y, startX, startY);
                    activeApp->onEvent(e);
                }
            }
        }
        prevX = 0;
        prevY = 0;
    }

    if (menu == Menu::Home) {
        drawTime();
    } else if (menu == Menu::App) {
        if (!activeApp) {
            // If no app is active, then fallback to the home menu
            menu = Menu::Home;
        } else {
            // Update and render the active app
            activeApp->update();
            activeApp->render();
        }
    }

    // After 5 seconds, shutdown
    int timeDiff = ttgo->rtc->getDateTime().second - sleepTimer;
    if (timeDiff < 0) timeDiff += 60;

    if (timeDiff > 5) {
        ttgo->displaySleep();
        ttgo->power->setPowerOutPut(AXP202_LDO3, false);
        ttgo->power->setPowerOutPut(AXP202_LDO4, false);
        ttgo->power->setPowerOutPut(AXP202_LDO2, false);
        // The following power channels are not used
        ttgo->power->setPowerOutPut(AXP202_EXTEN, false);
        ttgo->power->setPowerOutPut(AXP202_DCDC2, false);

        esp_sleep_enable_ext1_wakeup(GPIO_SEL_35, ESP_EXT1_WAKEUP_ALL_LOW);
        esp_deep_sleep_start();
    }
    
    prevPressing = pressing;

    lv_task_handler();
}
