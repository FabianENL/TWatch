#define LILYGO_WATCH_2020_V3
#include <LilyGoWatch.h>
#include <vector>

#include "app.h"
#include "apps.h"

TTGOClass *ttgo;
int updateTimer = 0;
bool irq = false;

std::vector<App*> apps = {new FlappyBird()};

void drawTime(){
	ttgo->tft->setTextColor(TFT_WHITE, TFT_BLACK);
	ttgo->tft->setTextSize(2);
	ttgo->tft->setCursor(0, 0);
	ttgo->tft->print(ttgo->power->getBattPercentage());
	ttgo->tft->print("%");
	ttgo->tft->setTextSize(1);
	ttgo->tft->drawString(ttgo->rtc->formatDateTime(PCF_TIMEFORMAT_HM), 40, 60, 7);
	updateTimer = ttgo->rtc->getDateTime().second;
}

void setup() {
    Serial.begin(115200);
	
    ttgo = TTGOClass::getWatch();
    ttgo->begin();

    ttgo->openBL();
    ttgo->setBrightness(128);       // 0~255

    drawTime();

    updateTimer = ttgo->rtc->getDateTime().second;

  	// button interrupt
    pinMode(AXP202_INT, INPUT_PULLUP);

    attachInterrupt(AXP202_INT, [] {
        irq = true;
    }, FALLING);
    ttgo->power->enableIRQ(AXP202_PEK_SHORTPRESS_IRQ, true);
    //  Clear interrupt status
    ttgo->power->clearIRQ();

    ttgo->motor_begin();

}

void loop() {
	ttgo->power->clearIRQ();

	for (App* app : apps) {
		app->update();
	}

	for (App* app : apps) {
		app->render();
	}
}
