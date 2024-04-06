#pragma once

#include "core.h"

class List {
public:
    typedef std::function<void(String)> list_event_cb;

    List()
    {
    }

    ~List()
    {
        if (m_listObj == nullptr)
            return;

        lv_obj_del(m_listObj);
        m_listObj = nullptr;
    }

    void init(lv_obj_t* parent)
    {
        static lv_style_t listStyle;
        lv_style_init(&listStyle);
        lv_style_set_radius(&listStyle, LV_OBJ_PART_MAIN, 0);
        lv_style_set_bg_color(&listStyle, LV_OBJ_PART_MAIN, LV_COLOR_GRAY);
        lv_style_set_bg_opa(&listStyle, LV_OBJ_PART_MAIN, LV_OPA_0);
        lv_style_set_border_width(&listStyle, LV_OBJ_PART_MAIN, 0);
        lv_style_set_text_color(&listStyle, LV_OBJ_PART_MAIN, LV_COLOR_WHITE);
        lv_style_set_image_recolor(&listStyle, LV_OBJ_PART_MAIN, LV_COLOR_WHITE);

        m_listObj = lv_list_create(parent, NULL);
        lv_list_set_scrollbar_mode(m_listObj, LV_SCROLLBAR_MODE_OFF);
        lv_obj_set_size(m_listObj, LV_HOR_RES, LV_VER_RES - 30);
        lv_obj_add_style(m_listObj, LV_OBJ_PART_MAIN, &listStyle);
        lv_obj_align(m_listObj, parent, LV_ALIGN_CENTER, 0, 0);
    }

    void add(String txt, void* imgsrc = (void*)LV_SYMBOL_IMAGE)
    {
        lv_obj_t* btn = lv_list_add_btn(m_listObj, imgsrc, txt.c_str());
        btn->user_data = this;
        lv_obj_set_event_cb(btn, callback);
    }

    void align(const lv_obj_t* base, lv_align_t align, lv_coord_t x = 0, lv_coord_t y = 0) { lv_obj_align(m_listObj, base, align, x, y); }
    void hidden(bool en = true) { lv_obj_set_hidden(m_listObj, en); }
    void setListCallback(list_event_cb cb) { m_eventCallback = cb; }

    lv_obj_t* getObj() const { return m_listObj; }

private:
    static void callback(lv_obj_t* obj, lv_event_t event)
    {
        if (event == LV_EVENT_SHORT_CLICKED) {
            List* list = static_cast<List*>(obj->user_data);
            if (list == nullptr) {
                Serial.println("List instance is nullptr");
                return;
            }

            String txt = lv_list_get_btn_text(obj);
            if (list->m_eventCallback != nullptr)
                list->m_eventCallback(txt);
        }
    }

private:
    lv_obj_t* m_listObj = nullptr;
    list_event_cb m_eventCallback = nullptr;
};