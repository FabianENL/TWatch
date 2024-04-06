#include "apps/settings.h"

lv_obj_t* createSettingPanel(lv_obj_t* parent)
{
    lv_obj_t* panel = lv_obj_create(parent, NULL);
    lv_obj_set_size(panel, LV_HOR_RES, 40);
    lv_obj_set_style_local_bg_opa(panel, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);
    lv_obj_set_style_local_border_opa(panel, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);
    lv_obj_align(panel, parent, LV_ALIGN_CENTER, 0, 0);
    return panel;
}

void Settings::init()
{
    m_titleLabel = lv_label_create(getParent(), NULL);
    lv_label_set_text(m_titleLabel, "Settings");
    lv_obj_align(m_titleLabel, getParent(), LV_ALIGN_IN_TOP_MID, 0, 10);

    m_settingsList = lv_list_create(getParent(), NULL);
    lv_obj_set_size(m_settingsList, LV_HOR_RES, LV_VER_RES - 35);
    lv_obj_align(m_settingsList, getParent(), LV_ALIGN_IN_TOP_MID, 0, 35);
    lv_obj_set_style_local_border_width(m_settingsList, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_radius(m_settingsList, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_list_set_scrollbar_mode(m_settingsList, LV_SCROLLBAR_MODE_OFF);

    m_settings.push_back(new BooleanSetting(createSettingPanel(m_settingsList), "Power saver", false));
}

void Settings::update()
{
}
