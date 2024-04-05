#include "core.h"

#include "app.h"
#include "apps/apps.h"
#include "elements/clock.h"
#include "event/inputevent.h"

enum {
    Q_EVENT_AXP_INT,
    Q_EVENT_ON
};

enum class Menu {
    None = 0,
    Home,
    App
};

LV_IMG_DECLARE(batman);

QueueHandle_t g_event_queue_handle = NULL;

int updateTimer = 0;
int sleepTimer = 0;
bool irq = false;
int16_t scrollY;
int16_t prevX, prevY;
Menu menu = Menu::Home;
ClockDisplay clockObj;

bool lenergy = false;
bool pressing = false;
bool prevPressing = false;
int16_t startX, startY;

App* activeApp = nullptr;
lv_obj_t* background = nullptr;

void home()
{
    menu = Menu::Home;
    if (activeApp) {
        for (lv_obj_t* obj : activeApp->getObjects()) {
            lv_obj_set_hidden(obj, true);
        }
    }
}

void turn_on()
{
    sleepTimer = ttgo->rtc->getDateTime().second;
    ttgo->startLvglTick();
    ttgo->displayWakeup();
    ttgo->rtc->syncToSystem();
    home();
    ttgo->openBL();
}

void turn_off()
{
    ttgo->closeBL();
    ttgo->stopLvglTick();
    ttgo->displaySleep();
}

void setup()
{
    Serial.begin(115200);

    g_event_queue_handle = xQueueCreate(20, sizeof(uint8_t));

    ttgo = TTGOClass::getWatch();
    ttgo->begin(); // Initialize the ttgo library
    ttgo->lvgl_begin(); // Initialize the lvgl library
    ttgo->motor_begin(); // Initialize the vibration motor
    ttgo->openBL(); // Enable the backlight
    ttgo->bl->adjust(25);

    sleepTimer = ttgo->rtc->getDateTime().second;
    ttgo->power->adc1Enable(AXP202_BATT_VOL_ADC1 | AXP202_BATT_CUR_ADC1 | AXP202_VBUS_VOL_ADC1 | AXP202_VBUS_CUR_ADC1, AXP202_ON);
    ttgo->power->enableIRQ(AXP202_VBUS_REMOVED_IRQ | AXP202_VBUS_CONNECT_IRQ | AXP202_CHARGING_FINISHED_IRQ, AXP202_ON);
    ttgo->power->clearIRQ();

    pinMode(AXP202_INT, INPUT);
    attachInterrupt(
        AXP202_INT, [] {
            uint8_t data = Q_EVENT_AXP_INT;
            BaseType_t xHigherPriorityTaskWoken = pdFALSE;
            xQueueSendFromISR(g_event_queue_handle, &data, &xHigherPriorityTaskWoken);
            if (xHigherPriorityTaskWoken)
                portYIELD_FROM_ISR();
        },
        FALLING);

    pinMode(TOUCH_INT, INPUT);
    attachInterrupt(
        TOUCH_INT, [] {
            if (!ttgo->bl->isOn()) {
                uint8_t data = Q_EVENT_ON;
                BaseType_t xHigherPriorityTaskWoken = pdFALSE;
                xQueueSendFromISR(g_event_queue_handle, &data, &xHigherPriorityTaskWoken);
                if (xHigherPriorityTaskWoken)
                    portYIELD_FROM_ISR();
            }
        },
        FALLING);

    // Check if the RTC clock matches, if not, use compile time
    ttgo->rtc->check();

    // Synchronize time to system time
    ttgo->rtc->syncToSystem();

    background = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_src(background, &batman);
    lv_obj_align(background, NULL, LV_ALIGN_CENTER, 0, 0);

    clockObj.init(background, LV_ALIGN_IN_TOP_RIGHT, -15, 20);

    for (App* app : Apps::apps) {
        app->init();

        // Hide all the generated objects by the app
        for (lv_obj_t* obj : app->getObjects()) {
            lv_obj_set_hidden(obj, true);
        }
    }
}

void loop()
{
    ttgo->power->clearIRQ();

    uint8_t data;
    if (xQueueReceive(g_event_queue_handle, &data, 5 / portTICK_RATE_MS) == pdPASS) {
        switch (data) {
        case Q_EVENT_AXP_INT:
            turn_on();
            return;
        case Q_EVENT_ON:
            turn_on();
            delay(200);
            break;
        default:
            break;
        }
    }

    if (!ttgo->bl->isOn())
        return;

    // After 5 seconds, shutdown
    int timeDiff = ttgo->rtc->getDateTime().second - sleepTimer;
    if (timeDiff < 0)
        timeDiff += 60;

    if (timeDiff > 5) {
        turn_off();
        return;
    }

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

        prevX = x;
        prevY = y;
    } else {
        if (prevPressing) {
            // On Release Event
            if (menu == Menu::Home) {
                Serial.println("Opening apps");
                activeApp = Apps::apps[0];
                menu = Menu::App;
            }

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
    prevPressing = pressing;

    if (menu == Menu::Home) {
        // drawTime();
        clockObj.show();
        lv_obj_set_hidden(background, false);
    } else if (menu == Menu::App) {
        clockObj.hide();
        lv_obj_set_hidden(background, true);
        if (!activeApp) {
            // If no app is active, then fallback to the home menu
            home();
        } else {
            // Update the active app
            for (lv_obj_t* obj : activeApp->getObjects()) {
                lv_obj_set_hidden(obj, false);
            }

            activeApp->update();
        }
    }

    lv_task_handler();
}
