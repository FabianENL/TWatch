#pragma once

#include "core.h"

class Battery {
public:
    ~Battery()
    {
        lv_obj_del(m_panel);
        lv_obj_del(m_image);
        lv_obj_del(m_percentage);
    }

    void init(lv_obj_t* parent, lv_align_t align = LV_ALIGN_CENTER, lv_coord_t x = 0, lv_coord_t y = 0)
    {
        m_panel = lv_obj_create(parent, NULL);
        lv_obj_set_size(m_panel, 70, 20);
        lv_obj_set_style_local_bg_opa(m_panel, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);
        lv_obj_set_style_local_border_opa(m_panel, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);
        lv_obj_align(m_panel, parent, align, x, y);

        m_percentage = lv_label_create(m_panel, NULL);
        lv_label_set_text(m_percentage, "100%");
        lv_obj_align(m_percentage, m_panel, LV_ALIGN_CENTER, 10, 0);

        m_image = lv_img_create(m_panel, NULL);
        lv_img_set_src(m_image, LV_SYMBOL_BATTERY_FULL);
        lv_obj_align(m_image, m_panel, LV_ALIGN_CENTER, -20, 0);

        lv_task_create(updateBattery, 30000, LV_TASK_PRIO_LOWEST, this);
    }

private:
    static void updateBattery(lv_task_t* task)
    {
        Battery* battery = static_cast<Battery*>(task->user_data);
        if (battery == nullptr) {
            Serial.println("Battery instance is nullptr");
            return;
        }

        int level = ttgo->power->getBattPercentage();
        const char* icon;
        if (ttgo->power->isChargeing()) {
            icon = LV_SYMBOL_CHARGE;
        } else {
            if (level > 95)
                icon = LV_SYMBOL_BATTERY_FULL;
            else if (level > 80)
                icon = LV_SYMBOL_BATTERY_3;
            else if (level > 45)
                icon = LV_SYMBOL_BATTERY_2;
            else if (level > 20)
                icon = LV_SYMBOL_BATTERY_1;
            else
                icon = LV_SYMBOL_BATTERY_EMPTY;
        }

        lv_img_set_src(battery->m_image, icon);
        lv_label_set_text_fmt(battery->m_percentage, "%d%%", level);
    }

private:
    lv_obj_t* m_panel = nullptr;
    lv_obj_t* m_image = nullptr;
    lv_obj_t* m_percentage = nullptr;
};