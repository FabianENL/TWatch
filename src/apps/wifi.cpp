#include "apps/wifi.h"

#include "apps/settings.h"
#include "elements/keyboard.h"

void Wifi::init()
{
    m_list.init(getParent());
    lv_obj_set_size(m_list.getObj(), LV_HOR_RES, LV_VER_RES - 45);
    lv_obj_align(m_list.getObj(), getParent(), LV_ALIGN_IN_TOP_MID, 0, 0);
    m_list.setCallback(BIND_FN(Wifi::listCallback));

    m_scanBtn = lv_btn_create(getParent(), NULL);
    m_scanBtn->user_data = this;
    lv_obj_align(m_scanBtn, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);
    lv_obj_set_event_cb(m_scanBtn, [](lv_obj_t* obj, lv_event_t event) {
        if (event == LV_EVENT_CLICKED) {
            Settings::forceOn = true;
            Wifi* wifiApp = (Wifi*)obj->user_data;
            lv_obj_set_hidden(wifiApp->m_scanningLabel, false);
            wifiApp->m_list.clear();
            WiFi.scanNetworks(true);
        }
    });
    m_scanBtnLabel = lv_label_create(m_scanBtn, NULL);
    lv_label_set_text(m_scanBtnLabel, "Scan");

    m_scanningLabel = lv_label_create(getParent(), NULL);
    lv_label_set_text(m_scanningLabel, "Scanning...");
    lv_obj_align(m_scanningLabel, getParent(), LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_hidden(m_scanningLabel, true);

    m_passwordTextarea = lv_textarea_create(getParent(), NULL);
    lv_obj_set_size(m_passwordTextarea, LV_HOR_RES - 40, 30);
    lv_obj_set_hidden(m_passwordTextarea, true);
    lv_obj_align(m_passwordTextarea, getParent(), LV_ALIGN_IN_TOP_MID, 0, 20);

    WiFi.mode(WIFI_STA);

    WiFi.onEvent([this](WiFiEvent_t event, WiFiEventInfo_t info) {
        Settings::forceOn = false;
        lv_obj_set_hidden(m_scanningLabel, true);
        int16_t len = WiFi.scanComplete();
        for (int16_t i = 0; i < len; i++) {
            m_list.add(WiFi.SSID(i), (void*)LV_SYMBOL_WIFI);
        }
    },
        WiFiEvent_t::ARDUINO_EVENT_WIFI_SCAN_DONE);
}

void Wifi::listCallback(String txt)
{
    Serial.printf("Connecting to %s", txt);
    lv_obj_set_hidden(m_passwordTextarea, false);
    Keyboard::get()->setFocus(m_passwordTextarea);
}
