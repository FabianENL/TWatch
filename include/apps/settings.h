#pragma once

#include "app.h"
#include "elements/switch.h"

#include <vector>

class Setting {
public:
    Setting(lv_obj_t* parent, String name)
        : m_name(name)
    {
        m_label = lv_label_create(parent, NULL);
        lv_label_set_text(m_label, name.c_str());
        lv_obj_align(m_label, parent, LV_ALIGN_IN_LEFT_MID, 10, 0);
    }

    ~Setting()
    {
        lv_obj_del(m_label);
    }

private:
    String m_name;
    lv_obj_t* m_label;
};

class BooleanSetting : public Setting {
public:
    BooleanSetting(lv_obj_t* parent, String name, bool defaultValue = false)
        : Setting(parent, name)
    {
        m_sw.init(parent);
        m_sw.setState(defaultValue);
        lv_obj_align(m_sw.getObj(), parent, LV_ALIGN_IN_RIGHT_MID, -10, 0);
    }

    bool getValue() const { return m_sw.getState(); }

private:
    Switch m_sw;
};

class Settings : public App {
public:
    Settings()
        : App("Settings")
    {
    }

    ~Settings()
    {
        for (Setting* setting : m_settings) {
            delete setting;
        }
        lv_obj_del(m_titleLabel);
        lv_obj_del(m_settingsList);
    }

    virtual void init() override;
    virtual void update() override;

private:
    std::vector<Setting*> m_settings;
    lv_obj_t* m_titleLabel;
    lv_obj_t* m_settingsList;
};