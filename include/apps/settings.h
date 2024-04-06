#pragma once

#include "app.h"
#include "elements/switch.h"

#include <vector>
#include <FS.h>
#include <SPIFFS.h>

struct Config {
    bool powerSaver;

    void save();
    void load();
};

class Setting {
public:
    Setting(String name)
        : m_name(name)
    {
    }

    ~Setting()
    {
        lv_obj_del(m_label);
    }

    virtual void init(lv_obj_t* parent)
    {
        m_label = lv_label_create(parent, NULL);
        lv_label_set_text(m_label, m_name.c_str());
        lv_obj_align(m_label, parent, LV_ALIGN_IN_LEFT_MID, 10, 0);
    }

private:
    String m_name;
    lv_obj_t* m_label;
};

class BooleanSetting : public Setting {
public:
    BooleanSetting(String name, Switch::switch_event_cb callback)
        : Setting(name)
        , m_callback(callback)
    {
    }

    virtual void init(lv_obj_t* parent) override
    {
        Setting::init(parent);
        m_sw.init(parent);
        lv_obj_align(m_sw.getObj(), parent, LV_ALIGN_IN_RIGHT_MID, -10, 0);
        m_sw.setCallback(m_callback);
    }

    bool getValue() const { return m_sw.getState(); }
    void setValue(bool value) { m_sw.setState(value); }

private:
    Switch m_sw;
    Switch::switch_event_cb m_callback;
};

class Settings : public App {
public:
    static Config config;

    Settings()
        : App("Settings")
    {
    }

    ~Settings()
    {
        lv_obj_del(m_titleLabel);
        lv_obj_del(m_settingsList);
    }

    virtual void init() override;
    virtual void update() override;

private:
    lv_obj_t* m_titleLabel;
    lv_obj_t* m_settingsList;

    BooleanSetting m_powerSaver = BooleanSetting("Power saver", [](bool checked) { config.powerSaver = checked; config.save(); });
};