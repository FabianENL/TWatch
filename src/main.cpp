#include "core.h"

#include "app.h"
#include "apps/apps.h"
#include "apps/settings.h"
#include "elements/battery.h"
#include "elements/clock.h"
#include "elements/switch.h"
#include "elements/keyboard.h"
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

int sleepTimer = 0;
int16_t prevX, prevY;
Menu menu = Menu::Home;
ClockDisplay clockObj;
Battery battery;

bool pressing = false;
bool prevPressing = false;
int16_t startX, startY;

lv_obj_t* background = nullptr;

void home()
{
    menu = Menu::Home;
    Settings::forceOn = false;
    if (Apps::activeApp)
        lv_obj_set_hidden(Apps::activeApp->getParent(), true);
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
    Keyboard::get()->setFocus(NULL);
    if (Settings::config.powerSaver) {
		ttgo->displaySleep();
		ttgo->power->setPowerOutPut(AXP202_LDO3, false);
		ttgo->power->setPowerOutPut(AXP202_LDO4, false);
		ttgo->power->setPowerOutPut(AXP202_LDO2, false);
		// The following power channels are not used
		ttgo->power->setPowerOutPut(AXP202_EXTEN, false);
		ttgo->power->setPowerOutPut(AXP202_DCDC2, false);

		esp_sleep_enable_ext1_wakeup(GPIO_SEL_35, ESP_EXT1_WAKEUP_ALL_LOW);
		esp_deep_sleep_start();
    } else {
        ttgo->closeBL();
        ttgo->stopLvglTick();
        ttgo->displaySleep();
    }
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

    // Dark theme
    lv_theme_t* th = lv_theme_material_init(
        LV_THEME_DEFAULT_COLOR_PRIMARY,
        LV_THEME_DEFAULT_COLOR_SECONDARY,
        LV_THEME_MATERIAL_FLAG_DARK,
        lv_theme_get_font_small(),
        lv_theme_get_font_normal(),
        lv_theme_get_font_subtitle(),
        lv_theme_get_font_title());
    lv_theme_set_act(th);

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

    // Check if the RTC clock matches, if not, use compile time
    ttgo->rtc->check();

    // Synchronize time to system time
    ttgo->rtc->syncToSystem();

    background = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_src(background, &batman);
    lv_obj_align(background, NULL, LV_ALIGN_CENTER, 0, 0);

    clockObj.init(background, LV_ALIGN_IN_TOP_RIGHT, -15, 20);
    battery.init(background, LV_ALIGN_IN_BOTTOM_LEFT);
    Keyboard::get()->init();

    for (App* app : Apps::apps) {
        lv_obj_t* parent = lv_obj_create(lv_scr_act(), NULL);
        lv_obj_set_style_local_border_width(parent, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
        lv_obj_set_style_local_radius(parent, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
        lv_obj_set_size(parent, 240, 240);
        lv_obj_set_hidden(parent, true);
        app->preinit(parent);
        app->init();
    }
}

void loop()
{
    uint64_t start = millis();

    if (Settings::forceOn)
        sleepTimer = ttgo->rtc->getDateTime().second;

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

    uint32_t expected = lv_task_handler();

    // After 5 seconds, shutdown
    int timeDiff = ttgo->rtc->getDateTime().second - sleepTimer;
    if (timeDiff < 0)
        timeDiff += 60;

    if (timeDiff > 5 && !Settings::forceOn && !Keyboard::get()->isVisible()) {
        turn_off();
        return;
    }

    int16_t x, y;
    pressing = ttgo->getTouch(x, y);
    if (pressing) {
        if (!prevPressing) {
            // On Press event
            if (Apps::activeApp) {
                PressEvent e(x, y);
                Apps::activeApp->onEvent(e);
            }
            startX = x;
            startY = y;
        }

        if (x != prevX || y != prevY) {
            // On Move Event
            if (Apps::activeApp) {
                MoveEvent e(x, y, startX, startY);
                Apps::activeApp->onEvent(e);
            }
        }

        sleepTimer = ttgo->rtc->getDateTime().second;

        prevX = x;
        prevY = y;
    } else {
        if (prevPressing) {
            // On Release Event
            if (menu == Menu::Home) {
                Apps::activeApp = Apps::apps[0];
                Settings::forceOn = false;
                menu = Menu::App;
            }

            if (Apps::activeApp) {
                ReleaseEvent e(x, y, startX, startY);
                Apps::activeApp->onEvent(e);
            }

            int16_t dx = x - startX;
            int16_t dy = y - startY;
            // If the change of finger position is less than x amount pixels, then its a click
            if (abs(dx) < 10 && abs(dy) < 10) {
                // On Click Event
                if (Apps::activeApp) {
                    ClickEvent e(x, y);
                    Apps::activeApp->onEvent(e);
                }

            } else {
                // On Swipe Event
                if (Apps::activeApp) {
                    SwipeEvent e(x, y, startX, startY);
                    Apps::activeApp->onEvent(e);
                }
            }
        }
        prevX = 0;
        prevY = 0;
    }
    prevPressing = pressing;

    if (menu == Menu::Home) {
        Settings::forceOn = false;
        lv_obj_set_hidden(background, false);
        for (App* app : Apps::apps)
            lv_obj_set_hidden(app->getParent(), true);
    } else if (menu == Menu::App) {
        if (!Apps::activeApp) {
            // If no app is active, then fallback to the home menu
            home();
        } else {
            lv_obj_set_hidden(background, true);
            for (App* app : Apps::apps)
                if (app != Apps::activeApp)
                    lv_obj_set_hidden(app->getParent(), true);

            // Update the active app
            lv_obj_set_hidden(Apps::activeApp->getParent(), false);
            Apps::activeApp->update();
        }
    }

    int64_t duration = millis() - start;
    int32_t d = expected - duration;
    if (d > 0)
        delay(d);
}
