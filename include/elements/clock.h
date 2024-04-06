#pragma once

#include "core.h"

LV_FONT_DECLARE(morgnite_bold_64);

class ClockDisplay {
public:
    void init(lv_obj_t* parent, lv_align_t align = LV_ALIGN_CENTER, lv_coord_t x = 0, lv_coord_t y = 0)
    {
        RTC_Date curr_datetime = ttgo->rtc->getDateTime();

        static lv_style_t style;
        lv_style_init(&style);
        lv_style_set_text_font(&style, LV_STATE_DEFAULT, &morgnite_bold_64);

        m_panel = lv_obj_create(parent, nullptr);
        lv_obj_set_size(m_panel, 120, 60);
        lv_obj_set_style_local_bg_opa(m_panel, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);
        lv_obj_set_style_local_border_opa(m_panel, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);
        lv_obj_align(m_panel, parent, align, x, y);

        m_minute = lv_label_create(m_panel, nullptr);
        lv_obj_add_style(m_minute, LV_OBJ_PART_MAIN, &style);
        lv_label_set_text_fmt(m_minute, "%02u", curr_datetime.minute);
        lv_obj_align(m_minute, m_panel, LV_ALIGN_CENTER, 0, 0);

        m_hour = lv_label_create(m_panel, nullptr);
        lv_obj_add_style(m_hour, LV_OBJ_PART_MAIN, &style);
        lv_label_set_text_fmt(m_hour, "%02u", curr_datetime.hour);
        lv_obj_align(m_hour, m_panel, LV_ALIGN_CENTER, -40, 0);

        m_second = lv_label_create(m_panel, nullptr);
        lv_obj_add_style(m_second, LV_OBJ_PART_MAIN, &style);
        lv_label_set_text_fmt(m_second, "%02u", curr_datetime.second);
        lv_obj_align(m_second, m_panel, LV_ALIGN_CENTER, 40, 0);

        lv_task_create(&ClockDisplay::updateClock, 1000, LV_TASK_PRIO_MID, this);
    }

    void hide() { lv_obj_set_hidden(m_panel, true); }
    void show() { lv_obj_set_hidden(m_panel, false); }

    lv_obj_t* getObject() const { return m_panel; }

private:
    static void updateClock(lv_task_t* task)
    {
        ClockDisplay* clock = static_cast<ClockDisplay*>(task->user_data);
        if (clock == nullptr) {
            Serial.println("Clock instance is nullptr");
            return;
        }

        RTC_Date curr_datetime = ttgo->rtc->getDateTime();
        lv_label_set_text_fmt(clock->m_second, "%02u", curr_datetime.second);
        lv_label_set_text_fmt(clock->m_minute, "%02u", curr_datetime.minute);
        lv_label_set_text_fmt(clock->m_hour, "%02u", curr_datetime.hour);
    }

private:
    lv_obj_t* m_panel = nullptr;
    lv_obj_t* m_hour = nullptr;
    lv_obj_t* m_minute = nullptr;
    lv_obj_t* m_second = nullptr;
};