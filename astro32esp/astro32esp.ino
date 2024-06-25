#include "FastRandom.hpp"
#include "background.h"
#include "sprites.hpp"
#include "title.hpp"
#include "SimpleScreenTexts.h"
#include "InputController.h"
#include "Scroller.h"

LGFX lcd;
SimpleScreenTexts* screenTexts;

#define ORIENTATION 3 // 3=POWER left 1=POWER right
#define USE_SERIAL_OUT

#define EXTRA_BOTTOM 28
#define MIN_NEXT_DIRECTION_MS 400
#define MAX_EGGS 4
#define SCORE_FOR_SURVINING 200

enum GameModes {
  HELLO,
  CALIBRATE,
  MENU,
  IN_GAME
};

const int SCREEN_LEFT = 5;
const int SCREEN_TOP = 37;
const int SCREEN_WIDTH = PLAYFIELD_WIDTH;
const int SCREEN_HEIGHT = PLAYFIELD_HEIGHT;

uint32_t globalCnt = 0;
long lastMillis;

uint8_t mode;
long nextMode = millis() + 30000;
uint8_t modeDone = 0xff;
uint8_t subMode = 0;
int daisyDx = 0;
int daisyDy = 0;
int daisyFenceCnt = -1;
int eggs;
int oldEggs;
int waitCorn;
int daisyWithFenceDelay;
long nextPossibleHorz = 0;
long nextPossibleVert = 0;
bool gameOver;

InputController* inputController;

Title* title;
BigDaisy* bigDaisy = NULL;
Daisy* daisy;
DaisyInPeaces *daisyInPeaces;
AbstractSprite* life;
Scroller* scroller;
Egg* eggSprite[MAX_EGGS];
GetReady* getReady;
Point lastDaisyPos;
long daisyDelay;
long eggDelay;

uint8_t miscMode;
uint8_t daisyMode = FLYING;
int daisyLifes;
int oldDaisyLifes;
int score;
int oldScore;
int survived;

LGFX_Sprite background;
String getAllHeap(){
  char temp[300];
  sprintf(temp, "Heap: Free:%i, Min:%i, Size:%i, Alloc:%i", ESP.getFreeHeap(), ESP.getMinFreeHeap(), ESP.getHeapSize(), ESP.getMaxAllocHeap());
  return temp;
}

bool isGlobal(uint8_t mask) {
  return (globalCnt & mask) == mask;
}

void clearBackground() {
  background.setSwapBytes(true);
  background.fillRect(0, 0, background.width(), background.height()-EXTRA_BOTTOM, TFT_BLACK);
  background.pushImage(-SCREEN_LEFT, background.height()-EXTRA_BOTTOM, lcd.width(), EXTRA_BOTTOM, &back_bottom[0]);
}

void drawPlayfield() {
  background.pushSprite(&lcd, SCREEN_LEFT, SCREEN_TOP);
}

void restoreBg() {
  lcd.pushImage(0, 0, lcd.width(), 38, &back_top[0]);
  lcd.pushImage(0, lcd.height()-55, lcd.width(), 54, &back_bottom[0]);
  for(int n=38; n<185; n+=2) {
    lcd.pushImage(0,n, 5, 2, &back_left[0]);
    lcd.pushImage(lcd.width()-5, n, 5, 2, &back_right[0]);
  }
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
    screenTexts->bigText("The VincentGlueck", TFT_WHITE);
    screenTexts->smallText("presents");
    modeDone = mode;
    nextMode = millis() + 3000;
    subMode = 0;
  }
  if((subMode == 0) && (millis() > (nextMode-2300))) {
    subMode = 1;
    lcd.clearDisplay(TFT_BLACK);
    screenTexts->smallText("in association with", TFT_SILVER, -35);
  }
  if((subMode == 1) && (millis() > (nextMode-1400))) {
    screenTexts->bigText("The Hase", TFT_WHITE);
    subMode = 2;
  }
  inputController->poll();
  if(inputController->getInput() != Nothing) {
    mode = MENU;
    inputController->processed();
  }
}

void calibrateTouch() {
  if (modeDone != mode) {
    restoreBg();
    bigDaisy = new BigDaisy();
    bigDaisy->setPos(Point(-120, 40));
    modeDone = mode;
    miscMode = 0;
    nextMode = millis() + 5000;
  }
  clearBackground();
  bigDaisy->drawOnSprite(&background);
  bigDaisy->onTick();
  screenTexts->spriteBig("Calibrate touch?", background, TFT_WHITE);
  if ((millis() + 3500) > nextMode) {
    screenTexts->spriteSmall("Touch anywhere...", background);
    miscMode = 1;
  }
  drawPlayfield();
  inputController->poll();
  if (inputController->getInput() != Nothing) {
    inputController->processed();
    inputController->calibrate();
  }
}

void menu() {
  if (modeDone != mode) {
    restoreBg();
    title = new Title();
    title->setPos(Point(320, 35));
    getReady = new GetReady();
    modeDone = mode;
    lastMillis = millis() + 3000;
  }
  clearBackground();
  scroller->onTick();
  title->drawAllOnSprite(&background);
  if(getReady->getStatus() == NORMAL) getReady->drawOnSprite(&background);
  getReady->onTick();
  title->onTick();
  drawPlayfield();
  if(millis() > daisyDelay) {
  inputController->poll();
    if (inputController->getInput() != Nothing) {
      delete getReady;
      delete title;
      mode = IN_GAME;
    }
  }
}

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);
  lcd.init();
  lcd.setRotation(ORIENTATION);
  lcd.setSwapBytes(true);
  if(!initBackground()) {
    while(true) {
      Serial.println("initBackground: Out of memory");
      sleep(10);
    }
  }
  screenTexts = new SimpleScreenTexts(lcd);
  inputController = new InputController(lcd);
  life = new Life();
  scroller = new Scroller(background);
  for(int n=0; n<MAX_EGGS; n++) eggSprite[n] = NULL;
  scroller->setSpeed(1); // TWO to make things weirder
  lastMillis = millis();
  mode = HELLO;
}

void testSprite() {
  bool flag = false;
  AbstractSprite* sprite = new Corn();
  sprite->setStatus(NORMAL);
  sprite->setPos(Point(20, 13));
  sprite->setUsrFlag0(flag);
  for(;;) {
    clearBackground();
    background.setColor(TFT_DARKGRAY);
    background.drawFastHLine(0, 50, 320);
    background.drawFastHLine(0, 60, 320);
    background.drawFastHLine(0, 70, 320);
    background.drawFastHLine(0, 80, 320);
    background.drawFastHLine(0, 90, 320);
    // background.drawFastHLine(0, 50, 320);
    for(int n=0; n<80; n+=79) {
      background.drawFastVLine(10+n, 0, 240);
      background.drawFastVLine(20+n, 0, 240);
      background.drawFastVLine(30+n, 0, 240);
      background.drawFastVLine(40+n, 0, 240);
      background.drawFastVLine(50+n, 0, 240);
    }
    sprite->drawOnSprite(&background);
    drawPlayfield();
    lcd.setTextColor(TFT_WHITE);
    lcd.drawString(String(sprite->getAnimCnt()), 20, 100, 4);
    sprite->onTick();
    flag = !flag;
    sprite->setAnimCnt(flag ? 1 : 0);
    sprite->setPos(Point(20, sprite->getPos().y));
    delay(100);
    do {
      inputController->poll();
      delay(50);
    } while (inputController->getInput() == Nothing);
    inputController->processed();
  }
}

void drawLifes() {
  if(oldDaisyLifes == daisyLifes) return;
  oldDaisyLifes = daisyLifes;
  lcd.fillRect(18, 224, 34, 12, TFT_BLACK);
  for(int n=0; n<daisyLifes; n++) {
    life->setPos(Point(18+(n*13), 224));
    life->drawOnLcd(&lcd);
  }
}

void drawEggs() {
  if(eggs == oldEggs) return;
  lcd.setTextColor(eggs >= 3 ? TFT_GREEN : (eggs < 1 ? TFT_RED : TFT_DARKCYAN));
  lcd.fillRect(172, 226, 16, 8, TFT_BLACK);
  lcd.drawString(String(eggs), 172, 226);
  oldEggs = eggs;
}

uint8_t getFreeEggSlot() {
  int n=0;
  while(n < MAX_EGGS) {
    if(eggSprite[n] == NULL) {
      return n;
    }
    n++;
  }
  Serial.println("No more eggs possible!");
  return -1;
}

void handleDaisy() {
  uint8_t input = inputController->getInput();
  if((input & Right) != 0) {
    if((millis() >= nextPossibleHorz) || (daisyDx != 0)) {
      if(daisyDx < 1) daisyDx++;
      nextPossibleHorz = millis() + MIN_NEXT_DIRECTION_MS;
    }
  }
  if((input & Left) == Left) {
    if((millis() >= nextPossibleHorz) || (daisyDx != 0)) {
      if(daisyDx > -2) daisyDx--;
      nextPossibleHorz = millis() + MIN_NEXT_DIRECTION_MS;
    }
  }
  if((input & Up) == Up) {
    if((millis() >= nextPossibleVert) || (daisyDy != 0)) {
      if(daisyDy > -2) daisyDy--;
      nextPossibleVert = millis() + MIN_NEXT_DIRECTION_MS;
    }
  }
  if((input & Down) != 0) {
    if((millis() >= nextPossibleVert) || (daisyDy != 0)) {
      if(daisyDy < 1) daisyDy++;
      nextPossibleVert = millis() + MIN_NEXT_DIRECTION_MS;
    }
  }
  int x = daisy->getPos().x;
  int y = daisy->getPos().y;
  x += daisyDx;
  y += daisyDy;
  
  if(x < 10) x = 10; else if (x > 170) x = 170;
  if(y < 5) y = 5; else if (y > 107) y = 107;
  daisy->setPos(Point(x, y));
  scroller->setDaisyPos(daisy->getPos(), daisyMode);
  
  if((input == Fire) && (eggs > 0) && (millis() > eggDelay)) {
    eggs--;
    eggDelay = millis() + 1500;
    int idx = getFreeEggSlot();
    if(idx != -1) {
      Egg* egg = new Egg();
      egg->setPos(Point(daisy->getPos().x + 20, daisy->getPos().y + 16));
      eggSprite[idx] = egg;
    } else Serial.println("No more free slots for eggs...");
  }
}

void backToLife() {
  daisyLifes--;
  if(daisyLifes <= 0) {
    gameOver = true;
    daisyDelay = millis() + 2000;
    return;
  }
  daisyMode = REBIRTH;
  daisyDelay = millis() + 1500;
  daisy->setPos(Point(lastDaisyPos.x, lastDaisyPos.y));
  if(eggs < 2) eggs = 2;
}

void daisyFlying() {
  if(daisy->getStatus() == VANISHED) return;
  if(daisy->getStatus() == NORMAL) daisy->drawOnSprite(&background);
  daisy->onTick();
  if(daisy->getStatus() != VANISHED) {
    if(scroller->getCollisionIdx(BULLET, daisy) >= 0) {
      daisyMode = HIT_BY_BULLET;
      daisyInPeaces = new DaisyInPeaces(daisy->getPos().x, daisy->getPos().y);
    }
    if (scroller->getCollisionIdx(DOG, daisy) >= 0) daisyMode = ATE_BY_DOG;
    if (scroller->getCollisionIdx(MILL, daisy) >= 0) daisyMode = MILL_CRASH;
    if (scroller->getCollisionIdx(WOLF, daisy) >= 0) daisyMode = WOLF_CRASH;
    if (scroller->getCollisionIdx(FENCE, daisy) >= 0) {
      daisyWithFenceDelay = 4;
      daisyFenceCnt = 200;
    } else if ((scroller->getCollisionIdx(CORN, daisy) >= 0) && (waitCorn <= 0)) {
      eggs++;
      if(eggs > 99) eggs = 99;
      waitCorn = 22;  // TODO: this is hacky
    }
    waitCorn--;
    if(daisyWithFenceDelay > 0) {
      daisyWithFenceDelay--;
      if(daisyWithFenceDelay == 0) daisy->setUsrFlag0(true);
    }
  } else {
    daisyDx = 0;
    daisyDy = 0;
  }
}

void daisyWasShot() {
  daisyMode = IN_PEACES;
  daisyInPeaces->onTick();
  daisyInPeaces->drawAllOnBackground(&background);
  if(daisyInPeaces->isReady() && (daisyInPeaces != NULL)) {
    delete daisyInPeaces;
    daisyDelay = millis() + 1000;
    backToLife();
    daisy->setPos(Point(0xffff, 0));
    scroller->setDaisyPos(daisy->getPos(), daisyMode);
  }
}

void daisyNextTry() {
  if(isGlobal(0xf)) {
    if(millis() > daisyDelay) {
      daisy->setStatus(NORMAL);
      daisyDx = 0;
      daisyDy = 0;
      inputController->processed();
      daisyMode = PROTECTED;
      daisyDelay = millis() + 1500;
    }
  }
}

void daisyAway() {
  daisy->setStatus(VANISHED);
  daisy->setPos(Point(0xffff, 0));
}

bool checkEggHit(int type, int spriteNum, int anim) {
  int idx = scroller->getCollisionIdx(type, eggSprite[spriteNum]);
  if(idx >= 0 && (scroller->getAbstractSprite(idx)->getStatus() != COLLIDED)) {
    scroller->getAbstractSprite(idx)->setStatus(COLLIDED);
    scroller->getAbstractSprite(idx)->setAnimCnt(anim);
    score+4;
    return true;
  }
  return false;
}

void mainGame() {
  if (modeDone != mode) {
    restoreBg();
    daisy = new Daisy();
    daisyDx = 0;
    daisyDy = 0;
    eggs = 3;
    oldEggs = -1;
    waitCorn = 0;
    daisyDelay = millis() + 1500;
    eggDelay = daisyDelay;
    daisyMode = FLYING;
    daisyLifes = 3;
    oldDaisyLifes = -1;
    score = 0;
    survived = 0;
    oldScore = -1;
    gameOver = false;
    inputController->getInput();
    inputController->processed();
    modeDone = mode;
  } else if(gameOver) {
    daisy->setStatus(VANISHED);
    clearBackground();
    scroller->onTick();
    drawEggs();
    if((globalCnt & 0xf) > 6) screenTexts->spriteBig("Game over", background, TFT_DARKGRAY);
    drawPlayfield();
    if(millis() > daisyDelay) {
      inputController->processed();
      inputController->poll();
      if(inputController->getInput() != Nothing) {
        inputController->processed();
        mode = MENU;
        daisyDelay = millis() + 1500;
      }
    }
  } else {
    clearBackground();
    scroller->onTick();
    if(daisyMode == FLYING || daisyMode == PROTECTED) {
      if(daisyMode == PROTECTED) daisy->setStatus((isGlobal(2) ? NORMAL : INVISIBLE));
      daisyFlying();
    } else if(daisyMode == HIT_BY_BULLET) {
      daisyAway();
      daisyWasShot();
    } else if(daisyMode == IN_PEACES) {
      daisyWasShot();
    } else if(daisyMode == MILL_CRASH) {
      daisyAway();
      backToLife();
    } else if(daisyMode == ATE_BY_DOG) {
      daisyAway();
      backToLife();
    } else if (daisyMode == WOLF_CRASH) {
      daisyAway();
      backToLife();
    } else if (daisyMode == GOT_FENCE) {
      daisy->setAnimCnt(daisy->getAnimCnt() + 3);
      daisyMode = FLYING;
    } else if (daisyMode == WOLF_CRASH) {
      daisyAway();
      backToLife();
    } else if(daisyMode == REBIRTH) {
      daisyNextTry();
    }
    if(daisyMode == PROTECTED && millis() > daisyDelay) {
      daisy->setStatus(NORMAL);
      daisyMode = FLYING;
    }
    for(int n=0; n<MAX_EGGS; n++) if (eggSprite[n] != NULL) eggSprite[n]->drawOnSprite(&background);
    drawEggs();
    drawPlayfield();
    drawLifes();
    inputController->processed();
    inputController->poll();
    handleDaisy();
    if(daisyMode == FLYING) {
      lastDaisyPos = Point(daisy->getPos().x, daisy->getPos().y);
    }
    if(daisyFenceCnt > 0) daisyFenceCnt--; else if (daisyFenceCnt == 0) {
      daisyFenceCnt--;
      daisy->setUsrFlag0(false);
      daisy->setAnimCnt(daisy->getAnimCnt()-3);
    }
    for(int n=0; n<MAX_EGGS; n++) {
      if(eggSprite[n] != NULL) {
        if(eggSprite[n]->getStatus() == VANISHED) {
          delete eggSprite[n];
          eggSprite[n] = NULL;
        } else {
          bool eggHit = false;
          eggSprite[n]->onTick();
          eggHit |= checkEggHit(DOG, n, 6);
          eggHit |= checkEggHit(HUNTER, n, 6);
          if(eggHit) {
            delete eggSprite[n];
            eggSprite[n] = NULL;
          }
        }
      }
    }
    if(survived > SCORE_FOR_SURVINING) {
      survived = 0;
      score++;
    }
  }
}

void loop() {
  // testSprite();
  if (millis() > nextMode) {
    if (mode < MENU) {
      mode++;
      if(mode == MENU) delete bigDaisy;         
    } else nextMode = 1 << 30;
  }
  long t=millis();
  if(mode == CALIBRATE) mode = MENU; // skip calibrate
  switch (mode) {
    case HELLO: hello(); break;
    case CALIBRATE: calibrateTouch(); break;
    case MENU: menu(); break;
    case IN_GAME: mainGame(); break;
    default:
      break;
  }
  do {
    yield();
  } while((millis()-16) < t);
  globalCnt++;
}