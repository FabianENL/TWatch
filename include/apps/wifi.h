#pragma once

#include "app.h"
#include "core.h"
#include "elements/list.h"

#include <WiFi.h>

enum class ButtonType {
    Scan,
};

class Wifi : public App {
public:
    Wifi()
        : App("Wifi")
    {
    }

    ~Wifi() {
        lv_obj_del(m_scanBtn);
        lv_obj_del(m_scanBtnLabel);
    }

    virtual void init() override;
private:
    void listCallback(String txt);

private:
    EventGroupHandle_t m_eventGroup = NULL;
    List m_list;
    lv_obj_t* m_scanBtn;
    lv_obj_t* m_scanBtnLabel;
    lv_obj_t* m_scanningLabel;
    lv_obj_t* m_passwordTextarea;
};