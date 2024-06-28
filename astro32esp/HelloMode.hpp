#ifndef _HELLO_MODE_HPP_
#define _HELLO_MODE_HPP_

#include "AbstractMode.h"
#include "SimpleScreenTexts.h"

class HelloMode : public AbstractMode {
private:
  uint8_t subMode;
  long autoSwitchMode;
  SimpleScreenTexts* screenTexts;

public:
  HelloMode(LGFX* _lcd, LGFX_Sprite* _background, InputController* _inputController)
    : AbstractMode(_lcd, _background, _inputController) {
    Serial.println("HelloMode constructor");
    currentMode = HELLO;
    subMode = 0;
    screenTexts = new SimpleScreenTexts(lcd);
  }

  ~HelloMode() {
    killMode();
  }

  void killMode() {
    Serial.println("HelloMode killed");
    delete screenTexts;
  }

  void onTick() {
    if (!initialized) {
      screenTexts->bigText("The VincentGlueck", TFT_WHITE);
      screenTexts->smallText("presents");
      initialized = true;
      autoSwitchMode = millis() + 3000;
    }
    if ((subMode == 0) && (millis() > (autoSwitchMode - 2300))) {
      subMode = 1;
      lcd->clearDisplay(TFT_BLACK);
      screenTexts->smallText("in association with", TFT_SILVER, -35);
    }
    if ((subMode == 1) && (millis() > (autoSwitchMode - 1400))) {
      screenTexts->bigText("The Hase", TFT_WHITE);
      subMode = 2;
    }
    inputController->poll();
    if ((millis() > autoSwitchMode) || inputController->isTouched()) {
      currentMode = MENU;
    }
  }
};

#endif