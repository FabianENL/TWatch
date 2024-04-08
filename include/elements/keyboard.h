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
    }

    void setFocus(lv_obj_t* textarea) const
    {
        if (textarea) {
            lv_obj_set_hidden(m_keyboard, false);
            lv_obj_move_foreground(m_keyboard);
        }
        lv_keyboard_set_textarea(m_keyboard, textarea);
    }

    bool isVisible() { return lv_obj_is_visible(m_keyboard); }

    static Keyboard* get() { return s_instance; }

private:
    static Keyboard* s_instance;
    lv_obj_t* m_keyboard;
};
