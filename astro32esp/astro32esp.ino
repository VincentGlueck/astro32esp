#include "InputController.h"
#include "Scroller.h"
#include "MenuMode.hpp"
#include "HelloMode.hpp"
#include "GameMode.hpp"

LGFX lcd;

#define ORIENTATION 3  // 3=POWER left 1=POWER right, you'll have to adjust InputController.cpp (calibration removed!)
#define USE_SERIAL_OUT

long lastMillis;

uint8_t mode = HELLO;

InputController* inputController;

Scroller* scroller;
AbstractMode* gameMode;

LGFX_Sprite background;

void drawPlayfield() {
  background.pushSprite(&lcd, SCREEN_LEFT, SCREEN_TOP);
}

bool initBackground() {
  if (!background.createSprite(SCREEN_WIDTH, SCREEN_HEIGHT)) {
    Serial.println("Unable to create background");
    return false;
  }
  return true;
}

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);
  lcd.init();
  lcd.setRotation(ORIENTATION);
  lcd.setSwapBytes(true);
  if (!initBackground()) {
    while (true) {
      Serial.println("initBackground: Out of memory");
      sleep(10);
    }
  }
  inputController = new InputController(&lcd);
  scroller = new Scroller(&background);
  scroller->setSpeed(1);  // TWO to make things weirder
  lastMillis = millis();
  mode = HELLO;
}

void loop() {
  long t = millis();
  if (gameMode != NULL) {
    if(mode != gameMode->getNextMode()) {
      mode = gameMode->getNextMode();
      gameMode->killMode();
      delete gameMode;
      switch (mode) {
        case HELLO: gameMode = new HelloMode(&lcd, &background, inputController); break;
        case MENU: gameMode = new MenuMode(&lcd, &background, inputController, scroller); break;
        case IN_GAME: gameMode = new GameMode(&lcd, &background, inputController, scroller); break;
        default: break;
      }
    } else {
      gameMode->onTick();
    }
  } else gameMode = new HelloMode(&lcd, &background, inputController); // one(!) mode should be created
  do {
    yield();
  } while ((millis() - 16) < t);
}