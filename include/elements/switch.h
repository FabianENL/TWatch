#pragma once

#include "core.h"

LV_IMG_DECLARE(on);
LV_IMG_DECLARE(off);
LV_IMG_DECLARE(iexit);

class Switch {
public:
    typedef std::function<void(bool)> switch_event_cb;

    ~Switch()
    {
        lv_obj_del(m_switchObj);
    }

    void init(lv_obj_t* parent, lv_align_t align = LV_ALIGN_CENTER, lv_coord_t x = 0, lv_coord_t y = 0)
    {
        m_switchObj = lv_switch_create(parent, NULL);
        m_switchObj->user_data = this;
        lv_obj_align(m_switchObj, parent, align, x, y);
        lv_obj_add_state(m_switchObj, LV_STATE_CHECKED);
        lv_obj_set_event_cb(m_switchObj, callback);
    }

    void setState(bool checked, lv_anim_enable_t anim = LV_ANIM_OFF)
    {
        if (checked)
            lv_switch_on(m_switchObj, anim);
        else
            lv_switch_off(m_switchObj, anim);
    }

    bool getState() const { return lv_switch_get_state(m_switchObj); }
    lv_obj_t* getObj() const { return m_switchObj; }

    void setCallback(switch_event_cb cb) { m_callback = cb; }

private:
    static void callback(lv_obj_t* obj, lv_event_t event)
    {
        if (event == LV_EVENT_VALUE_CHANGED) {
            Switch* sw = static_cast<Switch*>(obj->user_data);
            if (sw == nullptr) {
                Serial.println("Switch instance is nullptr");
                return;
            }

            bool checked = lv_switch_get_state(sw->m_switchObj);
            if (sw->m_callback != nullptr)
                sw->m_callback(checked);
        }
    }

private:
    switch_event_cb m_callback = nullptr;
    lv_obj_t* m_switchObj;
};