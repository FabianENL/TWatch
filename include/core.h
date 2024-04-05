#pragma once

#define LILYGO_WATCH_2020_V3
#define LILYGO_WATCH_LVGL
#include <LilyGoWatch.h>
#include <Arduino.h>

#define BIND_FN(fn) std::bind(&fn, this, std::placeholders::_1)

static TTGOClass* ttgo = nullptr;
