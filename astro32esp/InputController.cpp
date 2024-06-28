#include "InputController.h"

class LocalControllerCallback : public InputController::ControllerCallback {

private:
  long nextTouch;
  int userInput;
  bool calibrated;
  Point daisyPos;

public:
  void clicked(lgfx::touch_point_t *tp) {
    nextTouch = millis() + FASTEST_REPEAT_MS;
    int x;
    int y;
    if(calibrated) {
      x = tp->x;
      y = tp->y;
    } else {
      x = 330-abs(tp->x);
      y = 250-abs(tp->y);
    }
    userInput = Nothing;
    if(x > 180) userInput = Right; else if (x < 80) userInput = Left;
    if(y > 130) userInput = userInput | Down; else if (y < 70) userInput = userInput | Up;
    bool nearDaisy = (abs((x-50-daisyPos.x)) < 30) && (abs((y-50-daisyPos.y)) < 30);
    if(nearDaisy) {
      //Serial.printf("x: %d, y: %d, daisyX: %d, daisyY: %d\n", x, y, daisyPos.x, daisyPos.y);
      userInput = Fire;
    }
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

  void setDPos(Point _point) {
    daisyPos = Point(_point.x, _point.y);
  }
};

InputController::InputController(LGFX* _lcd) {
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

void InputController::setDaisyPos(Point _point) {
  callback->setDPos(_point);
}

uint8_t InputController::getInput() {
  return callback->getInput();
}

bool InputController::isTouched() {
  bool flag = touch->get(tp);
  processed();
  return flag;
}

void InputController::processed() {
  callback->processed();
}

void InputController::calibrate() {
  lcd->clearDisplay(TFT_BLACK);
  lcd->calibrate();
  lcd->clearDisplay(TFT_BLACK);
  screenTexts->bigText("Calibration", TFT_WHITE);
  screenTexts->smallText("done", TFT_WHITE);
  callback->setCalibrated(true);
  delay(1000);
}

void InputController::poll() {
  touch->poll();
}
