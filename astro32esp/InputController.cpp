#include "InputController.h"

class LocalControllerCallback : public InputController::ControllerCallback {

private:
  long nextTouch;
  long touchTime;
  UserInput userInput;

public:
  void clicked(lgfx::touch_point_t *tp) {
    if(millis() < nextTouch) {
      Serial.println("touch not accepted");
      return;
    }
    nextTouch = millis() + FASTEST_REPEAT_MS;
    Serial.printf("tp.x = %d, tp.y = %d, size = %d\n", tp->x, tp->y, tp->size);
    userInput = Fire;
  }

  UserInput getInput() {
    return userInput;
  }
};

InputController::InputController(LGFX _lcd) {
  lcd = _lcd;
  tp = (lgfx::touch_point_t *)malloc(sizeof(lgfx::touch_point_t) << 1);
  screenTexts = new SimpleScreenTexts(lcd);
  callback = new LocalControllerCallback();
  touch = new TheTouch(lcd, callback);
  isCalibrated = false;
  Serial.println("InputController initialized");
}

InputController::~InputController() {
  delete touch;
  delete callback;
  delete screenTexts;
  free(tp);
}

UserInput InputController::getInput() {
  return callback->getInput();
}

void InputController::calibrate() {
  lcd.clearDisplay(TFT_BLACK);
  lcd.calibrate();
  lcd.clearDisplay(TFT_BLACK);
  screenTexts->bigText("Calibration", TFT_WHITE);
  screenTexts->smallText("done", TFT_WHITE);
  isCalibrated = true;
  delay(1000);
}

void InputController::poll() {
  touch->poll();
}

void InputController::debugAsAscii() {
   switch (callback->getInput()) {
    case 0: break;
    case Up: Serial.println("Up"); break;
    case Left: Serial.println("Left"); break;
    case Right: Serial.println("Right"); break;
    case Down: Serial.println("Down"); break;
    case Fire: Serial.println("Fire"); break;
    default: break;
  }
}

