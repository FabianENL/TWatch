#define LILYGO_WATCH_2020_V3
#include <LilyGoWatch.h>
#include <vector>

#include "app.h"
#include "apps.h"

TTGOClass *ttgo;
int updateTimer = 0;
int sleepTimer = 0;
bool irq = false;

std::vector<App*> apps = { new FlappyBird() };

void drawTime(){
	ttgo->tft->setTextColor(TFT_WHITE, TFT_BLACK);
	ttgo->tft->setTextSize(2);
	ttgo->tft->setCursor(0, 0);
	ttgo->tft->print(ttgo->power->getBattPercentage());
	ttgo->tft->print("%");
	ttgo->tft->setTextSize(1);
	ttgo->tft->drawString(ttgo->rtc->formatDateTime(PCF_TIMEFORMAT_HM), 40, 60, 7);
}

void setup() {
    Serial.begin(115200);
	
    ttgo = TTGOClass::getWatch();
    ttgo->begin();
    ttgo->motor_begin();
    ttgo->openBL();
    ttgo->setBrightness(128);       // 0~255
    sleepTimer = ttgo->rtc->getDateTime().second;

  	// button interrupt
    pinMode(AXP202_INT, INPUT_PULLUP);
    attachInterrupt(AXP202_INT, [] {
        irq = true;
    }, FALLING);
    ttgo->power->enableIRQ(AXP202_PEK_SHORTPRESS_IRQ, true);
    //  Clear interrupt status
    ttgo->power->clearIRQ();

    drawTime();
}

void loop() {
	ttgo->power->clearIRQ();

	int16_t x, y;
	if (ttgo->getTouch(x, y)) {
		sleepTimer = ttgo->rtc->getDateTime().second;
	}

	int timeDiff = ttgo->rtc->getDateTime().second - sleepTimer;
	if(timeDiff < 0) timeDiff += 60;

	if (timeDiff > 5){
		ttgo->displaySleep();
		ttgo->power->setPowerOutPut(AXP202_LDO3, false);
		ttgo->power->setPowerOutPut(AXP202_LDO4, false);
		ttgo->power->setPowerOutPut(AXP202_LDO2, false);
		// The following power channels are not used
		ttgo->power->setPowerOutPut(AXP202_EXTEN, false);
		ttgo->power->setPowerOutPut(AXP202_DCDC2, false);

		esp_sleep_enable_ext1_wakeup(GPIO_SEL_35, ESP_EXT1_WAKEUP_ALL_LOW);
		esp_deep_sleep_start();
	}

	for (App* app : apps) {
		app->update();
	}

	for (App* app : apps) {
		app->render();
	}
}
