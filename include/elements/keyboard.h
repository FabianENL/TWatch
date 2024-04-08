#pragma once

#include "core.h"

class Keyboard {
public:
    ~Keyboard() { lv_obj_del(m_keyboard); }

    void init()
    {
        m_keyboard = lv_keyboard_create(lv_scr_act(), NULL);
        lv_keyboard_set_textarea(m_keyboard, NULL);
        lv_obj_set_hidden(m_keyboard, true);
        lv_event_cb_t original = lv_obj_get_event_cb(m_keyboard);
        m_keyboard->user_data = (void*)original;
        lv_obj_set_event_cb(m_keyboard, [](lv_obj_t* obj, lv_event_t event) {
            if (event == LV_EVENT_CANCEL) {
                Serial.println("Close keyboard");
            }
            if (event == LV_EVENT_APPLY) {
                Serial.println("Login");
            }
            ((lv_event_cb_t)obj->user_data)(obj, event);
        });
    }

    void setFocus(lv_obj_t* textarea) const
    {
        lv_obj_set_hidden(m_keyboard, false);
        lv_obj_move_foreground(m_keyboard);
        lv_keyboard_set_textarea(m_keyboard, textarea);
    }

    void unfocus()
    {
        lv_obj_set_hidden(m_keyboard, true);
        lv_keyboard_set_textarea(m_keyboard, NULL);
    }
    bool isVisible() { return lv_obj_is_visible(m_keyboard); }

    static Keyboard* get() { return s_instance; }

private:
    static Keyboard* s_instance;
    lv_obj_t* m_keyboard;
};
