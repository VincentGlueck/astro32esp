#include "InputController.h"

class LocalControllerCallback : public InputController::ControllerCallback {

private:
  long nextTouch;
  int userInput;
  bool calibrated;

public:
  void clicked(lgfx::touch_point_t *tp) {
    nextTouch = millis() + FASTEST_REPEAT_MS;
    int x;
    int y;
    if(calibrated) {
      x = tp->x;
      y = tp->y;
    } else {
      x = 330-tp->x;
      y = 250-tp->y;
    }
    //Serial.printf("input x: %d, y: %d\n", x, y);
    userInput = Nothing;
    bool moving = false;
    if(x > 180) {
      userInput = Right;
      moving = true;
    } else if (x < 50) {
      userInput = Left;
      moving = true;
    }
    if(y > 180) {
      userInput |= Down;
      moving = true;
    } else if (y < 60) {
      userInput |= Up;
      moving = true;
    }
    if(!moving) userInput = Fire;
  }

  uint8_t getInput() {
    return userInput;
  }

  void processed() {
    userInput = 0;
  }

  void setCalibrated(bool _cal) {
    calibrated = _cal;
  }
};

InputController::InputController(LGFX _lcd) {
  lcd = _lcd;
  tp = (lgfx::touch_point_t *)malloc(sizeof(lgfx::touch_point_t) << 1);
  screenTexts = new SimpleScreenTexts(lcd);
  callback = new LocalControllerCallback();
  touch = new TheTouch(lcd, callback);
}

InputController::~InputController() {
  delete touch;
  delete callback;
  delete screenTexts;
  free(tp);
}

uint8_t InputController::getInput() {
  return callback->getInput();
}

void InputController::processed() {
  callback->processed();
}

void InputController::calibrate() {
  lcd.clearDisplay(TFT_BLACK);
  lcd.calibrate();
  lcd.clearDisplay(TFT_BLACK);
  screenTexts->bigText("Calibration", TFT_WHITE);
  screenTexts->smallText("done", TFT_WHITE);
  callback->setCalibrated(true);
  delay(1000);
}

void InputController::poll() {
  touch->poll();
}
