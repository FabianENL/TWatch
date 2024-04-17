#pragma once

#include "core.h"
#include <WiFi.h>

class WifiElement {
public:
    ~WifiElement()
    {
        lv_obj_del(m_image);
    }

    void init(lv_obj_t* parent, lv_align_t align = LV_ALIGN_CENTER, lv_coord_t x = 0, lv_coord_t y = 0)
    {
        m_image = lv_img_create(parent, NULL);
        lv_img_set_src(m_image, LV_SYMBOL_WIFI);
        lv_obj_align(m_image, parent, align, x, y);
        lv_obj_set_style_local_image_recolor(m_image, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
        lv_obj_set_hidden(m_image, true);

        lv_task_create(updateWifi, 2000, LV_TASK_PRIO_MID, this);

        update();
    }

    void update()
    {
        lv_obj_set_hidden(m_image, !WiFi.isConnected());
    }

private:
    static void updateWifi(lv_task_t* task)
    {
        WifiElement* element = static_cast<WifiElement*>(task->user_data);
        element->update();
    }

private:
    lv_obj_t* m_image = nullptr;
};