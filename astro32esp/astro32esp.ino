#include "background.h"
#include "sprites.hpp"
#include "InputController.h"
#include "Scroller.h"
#include "MenuMode.hpp"
#include "HelloMode.hpp"
#include "GameMode.hpp"

LGFX lcd;

#define ORIENTATION 3  // 3=POWER left 1=POWER right
#define USE_SERIAL_OUT


#define SHOW_BTN_TIME 20

long lastMillis;

uint8_t mode = HELLO;

InputController* inputController;

BigDaisy* bigDaisy = NULL;
Scroller* scroller;
AbstractMode* gameMode;

LGFX_Sprite background;

String getAllHeap() {
  char temp[300];
  sprintf(temp, "Heap: Free:%i, Min:%i, Size:%i, Alloc:%i", ESP.getFreeHeap(), ESP.getMinFreeHeap(), ESP.getHeapSize(), ESP.getMaxAllocHeap());
  return temp;
}

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


void calibrateTouch() {
  /*
  if (modeDone != mode) {
    restoreBackground->restoreBg();
    bigDaisy = new BigDaisy();
    bigDaisy->setPos(Point(-120, 40));
    modeDone = mode;
    miscMode = 0;
    nextMode = millis() + 5000;
  }
  restoreBackground->clearPlayfield();
  bigDaisy->drawOnSprite(&background);
  bigDaisy->onTick();
  //screenTexts->spriteBig("Calibrate touch?", background, TFT_WHITE);
  if ((millis() + 3500) > nextMode) {
    //screenTexts->spriteSmall("Touch anywhere...", background);
    miscMode = 1;
  }
  drawPlayfield();
  inputController->poll();
  if (inputController->isTouched()) {
    inputController->calibrate();
  }
  */
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
  // testSprite();
  long t = millis();
  if (mode == CALIBRATE) mode = MENU;  // skip calibrate
  switch (mode) {
    case HELLO: break;
    case CALIBRATE: calibrateTouch(); break;
    case MENU: break;
    case IN_GAME: break;
    default:
      break;
  }
  if (gameMode != NULL) {
    if(mode != gameMode->getNextMode()) {
      mode = gameMode->getNextMode();
      gameMode->killMode();
      delete gameMode;
      Serial.printf("Switch game mode to %d\n", mode);
      if (mode == CALIBRATE) mode = MENU;  // skip calibrate
      switch (mode) {
        case HELLO: gameMode = new HelloMode(&lcd, &background, inputController); break;
        case CALIBRATE: break;
        case MENU: gameMode = new MenuMode(&lcd, &background, inputController, scroller); break;
        case IN_GAME: gameMode = new GameMode(&lcd, &background, inputController, scroller); break;
        default: break;
      }
      Serial.println(getAllHeap());
    } else {
      gameMode->onTick();
    }
  } else gameMode = new HelloMode(&lcd, &background, inputController);
  do {
    yield();
  } while ((millis() - 16) < t);
}