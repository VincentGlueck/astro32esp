#include "background.h"
#include "sprites.hpp"
#include "title.hpp"
#include "SimpleScreenTexts.h"
#include "InputController.h"

LGFX lcd;
SimpleScreenTexts* screenTexts;

#define ORIENTATION 3 // 3=POWER left 1=POWER right
#define USE_SERIAL_OUT

#define EXTRA_BOTTOM 28

const long TOUCH_REPEAT = 80;            // how often (ms) is (long)touch possible
const int SCREEN_LEFT = 5;
const int SCREEN_TOP = 37;
const int SCREEN_WIDTH = 310;
const int SCREEN_HEIGHT = 148 + EXTRA_BOTTOM;

uint32_t globalCnt = 0;
long lastMillis;

uint8_t mode = 0;
long nextMode = millis() + 30000;
uint8_t modeDone = 0xff;

lgfx::touch_point_t *touchPoint;
InputController* inputController;

Daisy* daisy;
Title* title;
Mill* mill;

uint8_t za, zb, zc, zx, counter;
uint8_t rnd() {
  zx++;
  za = (za ^ zc ^ zx);
  zb = (zb + za);
  zc = ((zc + (zb >> 1)) ^ za);
  return zc;
}

uint16_t score;

LGFX_Sprite background;


String getAllHeap(){
  char temp[300];
  sprintf(temp, "Heap: Free:%i, Min:%i, Size:%i, Alloc:%i", ESP.getFreeHeap(), ESP.getMinFreeHeap(), ESP.getHeapSize(), ESP.getMaxAllocHeap());
  return temp;
}


void clearBackground() {
  background.setSwapBytes(true);
  background.fillRect(0, 0, background.width(), background.height()-EXTRA_BOTTOM, TFT_BLACK);
  background.pushImage(-SCREEN_LEFT, background.height()-EXTRA_BOTTOM, lcd.width(), EXTRA_BOTTOM, &back_bottom[0]);
}

void restoreBg() {
  long t = millis();
  lcd.pushImage(0, 0, lcd.width(), 38, &back_top[0]);
  lcd.pushImage(0, lcd.height()-55, lcd.width(), 54, &back_bottom[0]);
  for(int n=38; n<185; n+=2) {
    lcd.pushImage(0,n, 5, 2, &back_left[0]);
    lcd.pushImage(lcd.width()-5, n, 5, 2, &back_right[0]);
  }
  t = millis() - t;
  Serial.printf("restoreBg() took %d ms\n", t);
}

bool initBackground() {
  if(!background.createSprite(SCREEN_WIDTH, SCREEN_HEIGHT)) {
    Serial.println("Unable to create background");
    return false;
  }
  return true;
}

void hello() {
  if (modeDone != mode) {
    screenTexts->bigText("VincentGlueck", TFT_WHITE);
    screenTexts->smallText("proudly presents");
    modeDone = mode;
    nextMode = millis() + 2000;
  }
}

void calibrateTouch() {
  if (modeDone != mode) {
    restoreBg();
    modeDone = mode;
    nextMode = millis() + 5000;
    mill->setStatus(NORMAL);
  }
  clearBackground();
  mill->onTick();
  if((mill->getStatus() == READY) && (rnd() & 0x3f) == 0x3f) {
    delete mill;
    mill = new Mill();
  }
  screenTexts->spriteBig("Calibrate touch?", background, TFT_WHITE);
  if ((millis() + 2000) > nextMode) screenTexts->spriteSmall("Touch anywhere...", background);
  mill->drawOnSprite(&background);
  background.pushSprite(&lcd, SCREEN_LEFT, SCREEN_TOP);
  inputController->poll();
  if (inputController->getInput() != Nothing) {
    inputController->calibrate();
  }
}

void menu() {
  // TODO there currently is no menu
  if (modeDone != mode) {
    // restoreBg();
    modeDone = mode;
    mode = 3;
  }
}

void gameOver() {
  if (mode != modeDone) {
    modeDone = mode;
  } else if ((globalCnt & 0x3fff) == 0x3fff) {
    int color = (globalCnt & 0x7fff) == 0x7fff ? TFT_WHITE : TFT_DARKGRAY;
    clearBackground();
    screenTexts->spriteBig("Game over", &background, color);
    background.pushSprite(&lcd, SCREEN_LEFT, SCREEN_TOP);    
  }
  if ((globalCnt & 0x7f) == 0x7f) {
    inputController->poll();
    if (inputController->getInput() != Nothing) {
      mode = 3;
    }
  }
}

void initSprites() {
  daisy = new Daisy();
  title = new Title();
  mill = new Mill();
  daisy->setPos(Point(-120, 20));
  title->setPos(Point(-5, 50));
  restoreBg();
}

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);
  lcd.init();
  lcd.setRotation(ORIENTATION);
  lcd.setSwapBytes(true);
  if(!initBackground()) {
    while(true) {
      Serial.println("Out of memory");
      sleep(4);
    }
  }
  za = random(256);
  zb = random(256);
  zc = random(256);
  zx = random(256);
  touchPoint = (lgfx::touch_point_t*)malloc(sizeof(lgfx::touch_point_t));
  screenTexts = new SimpleScreenTexts(lcd);
  inputController = new InputController(lcd);
  initSprites();
  lastMillis = millis();
}

void doTicks() {
  daisy->onTick();
  title->onTick();
  mill->onTick();
  if((mill->getStatus() == READY) && (rnd() & 0x3f) == 0x3f) {
    delete mill;
    mill = new Mill();
  }
}

void testSprite() {
  mill->setStatus(NORMAL);
  for(int n=0; n<50; n++) {
    mill->onTick();
    mill->setPos(Point(20, 10));
    clearBackground();
    mill->drawOnSprite(&background);
    background.pushSprite(&lcd, SCREEN_LEFT, SCREEN_TOP);
    lcd.setTextColor(TFT_WHITE);
    lcd.drawFastHLine(0, 40, 320);
    lcd.drawFastHLine(0, 50, 320);
    lcd.drawFastHLine(0, 60, 320);
    lcd.drawFastHLine(0, 70, 320);
    lcd.drawFastHLine(0, 80, 320);
    lcd.drawString(String(mill->getAnimCnt()), 200, 100, 4);
    delay(1000);
  }
}

void mainGame() {
  if (modeDone != mode) {
    restoreBg();
    modeDone = mode;
  } else {
    clearBackground();
    daisy->drawOnSprite(&background);
    mill->drawOnSprite(&background);
    title->drawAllOnSprite(&background);
    background.pushSprite(&lcd, SCREEN_LEFT, SCREEN_TOP);
    inputController->poll();
    doTicks();
  }
}

void loop() {
  if (millis() > nextMode) {
    if (mode < 2) mode++; else nextMode = 1 << 30;
    Serial.printf("mode now: %d\n", mode);
  }
  long t=millis();
  switch (mode) {
    case 0: hello(); break;
    case 1: calibrateTouch(); break;
    case 2: menu(); break;
    case 3: mainGame(); break;
    case 4: gameOver(); break;
    default:
      break;
  }
  do {
    yield();
  } while((millis()-16) < t);
  globalCnt++;
}