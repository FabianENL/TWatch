#pragma once

#include "core.h"

class Keyboard {
public:
    struct Data {
        lv_event_cb_t orig;
        lv_obj_t* textarea;
        std::function<void(String, bool)> callback;

        Data(lv_event_cb_t orig, lv_obj_t* textarea, std::function<void(String, bool)> callback)
            : orig(orig), textarea(textarea), callback(callback) {}
    };

    static void getText(std::function<void(String, bool)> callback)
    {
        lv_obj_t* keyboard = lv_keyboard_create(lv_scr_act(), NULL);
        
        lv_obj_t* textarea = lv_textarea_create(lv_scr_act(), NULL);
        lv_obj_set_size(textarea, LV_HOR_RES, LV_VER_RES / 2);
        lv_textarea_set_text(textarea, "");

        lv_event_cb_t event = lv_obj_get_event_cb(keyboard);
        Data* data = new Data(event, textarea, callback);
        keyboard->user_data = (void*)data;
        lv_obj_set_event_cb(keyboard, [](lv_obj_t* obj, lv_event_t event) {
            Data* data = (Data*)obj->user_data;
            data->orig(obj, event);
            if (event == LV_EVENT_CANCEL) {
                data->callback("", false);
                lv_obj_del(obj);
                lv_obj_del(data->textarea);
                delete data;
            } else if (event == LV_EVENT_APPLY) {
                data->callback(lv_textarea_get_text(data->textarea), true);
                lv_obj_del(obj);
                lv_obj_del(data->textarea);
                delete data;
            }
        });

        lv_keyboard_set_textarea(keyboard, textarea);
    }
};
