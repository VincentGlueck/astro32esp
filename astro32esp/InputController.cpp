#include "InputController.h"

class LocalControllerCallback : public InputController::ControllerCallback {
private:
  long nextTouch;
  int userInput;
  bool calibrated;
  Point daisyPos;
  Point latestTouch;
  long clearLatest = 0;
  int x;
  int y;

public:
  void clicked(lgfx::touch_point_t *tp) {
    nextTouch = millis() + FASTEST_REPEAT_MS;
    if(calibrated) {
      x = tp->x;
      y = tp->y;
    } else {
      x = 330-abs(tp->x);
      y = 235-abs(tp->y);
    }
    userInput = T_NOTHING;
    if(x > daisyPos.x+50) userInput = T_RIGHT; else if (x < (daisyPos.x-20)) userInput = T_LEFT;
    if(y > daisyPos.y+80) userInput = userInput | T_DOWN; else if (y < daisyPos.y+15) userInput = userInput | T_UP;
    bool nearDaisy = (abs((x-40-daisyPos.x)) < 30) && (abs((y-50-daisyPos.y)) < 40);
    if(nearDaisy) {
      userInput = T_FIRE;
    } else {
      Serial.printf("MISS -> x: %d, y: %d, daisyX: %d, daisyY: %d\n", x, y, daisyPos.x, daisyPos.y);
    }
    latestTouch = (userInput != T_NOTHING) ? Point(x, y) : Point(-1, -1);
  }

  uint8_t getInput() {
    return userInput;
  }

  void processed() {
    userInput = T_NOTHING;
  }

  void setCalibrated(bool _cal) {
    calibrated = _cal;
  }

  void setPPos(Point _point) {
    daisyPos = Point(_point.x, _point.y);
  }

  Point getLatestTouch() {
    if(millis() > clearLatest) {
      clearLatest = millis() + 500;
      latestTouch = Point(-1, -1);
    }
    return latestTouch;
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

void InputController::setPlayerPos(Point _point) {
  callback->setPPos(_point);
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

Point InputController::getLatestTouch() {
  return callback->getLatestTouch();
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
