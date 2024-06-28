#ifndef _GAME_MODE_HPP_
#define _GAME_MODE_HPP_

#include "AbstractMode.h"
#include "sprites.hpp"
#include "StyledNumber.hpp"

#define MAX_EGGS 4
#define MIN_NEXT_DIRECTION_MS 400
#define SCORE_FOR_SURVINING 200

class GameMode : public AbstractMode {
private:
  Egg* eggSprite[MAX_EGGS];
  AbstractSprite* life;
  Daisy* daisy;
  StyledNumber* eggsStyled;
  StyledNumber* scoreStyled;
  DaisyInPeaces* daisyInPeaces;
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
  LGFX_Sprite* gameOverSprite;
  int daisyLifes;
  int oldDaisyLifes;
  int score;
  int oldScore;
  int survived;
  uint8_t daisyMode = FLYING;
  Point lastDaisyPos;
  long daisyDelay;
  long eggDelay;
  uint32_t globalCnt = 0;

  bool isGlobal(uint8_t mask) {
    return (globalCnt & mask) == mask;
  }

  void drawLifes() {
    if (oldDaisyLifes == daisyLifes) return;
    oldDaisyLifes = daisyLifes;
    lcd->fillRect(18, 224, 34, 12, TFT_BLACK);
    for (int n = 0; n < daisyLifes; n++) {
      life->setPos(Point(18 + (n * 13), 224));
      life->drawOnLcd(lcd);
    }
  }

  void drawEggs() {
    if (eggs == oldEggs) return;
    eggsStyled->drawNumber(eggs, 170, 223);
    oldEggs = eggs;
  }

  void drawScore() {
    if(oldScore == score) return;
    scoreStyled->drawNumber(score, 250, 223);
    oldScore = score;
  }

  uint8_t getFreeEggSlot() {
    int n = 0;
    while (n < MAX_EGGS) {
      if (eggSprite[n] == NULL) {
        return n;
      }
      n++;
    }
    Serial.println("No more eggs possible!");
    return -1;
  }

  void handleDaisy() {
    uint8_t input = inputController->getInput();
    if ((input & Right) == Right) {
      if (millis() >= nextPossibleHorz) {
        if (daisyDx < 1) daisyDx++;
        Serial.printf("daisyDx: %d\n", daisyDx);
        nextPossibleHorz = millis() + MIN_NEXT_DIRECTION_MS;
      }
    }
    if ((input & Left) == Left) {
      if (millis() >= nextPossibleHorz) {
        if (daisyDx > -2) daisyDx--;
        // Serial.printf("daisyDx: %d\n", daisyDx);
        nextPossibleHorz = millis() + MIN_NEXT_DIRECTION_MS;
      }
    }
    if ((input & Up) == Up) {
      if ((millis() >= nextPossibleVert) || (daisyDy != 0)) {
        if (daisyDy > -2) daisyDy--;
        nextPossibleVert = millis() + MIN_NEXT_DIRECTION_MS;
      }
    }
    if ((input & Down) != 0) {
      if ((millis() >= nextPossibleVert) || (daisyDy != 0)) {
        if (daisyDy < 1) daisyDy++;
        nextPossibleVert = millis() + MIN_NEXT_DIRECTION_MS;
      }
    }
    int x = daisy->getPos().x;
    int y = daisy->getPos().y;
    x += daisyDx;
    y += daisyDy;
    if (x < 10) {
      x = 10;
      if (daisyDx > 0) daisyDx--;
    } else if (x > 170) {
      x = 170;
      daisyDx = 0;
    }
    if (y < 5) {
      y = 5;
      daisyDy = 0;
    } else if (y > 107) {
      y = 107;
      daisyDy = 0;
    }
    daisy->setPos(Point(x, y));
    inputController->setDaisyPos(daisy->getPos());
    scroller->setDaisyPos(daisy->getPos(), daisyMode);

    if ((input == Fire) && (eggs > 0) && (millis() > eggDelay)) {
      eggs--;
      eggDelay = millis() + 1500;
      int idx = getFreeEggSlot();
      if (idx != -1) {
        eggSprite[idx] = new Egg();
        eggSprite[idx]->setPos(Point(daisy->getPos().x + 20, daisy->getPos().y + 16));
      }
    }
  }

  void backToLife() {
    daisyAway();
    daisyLifes--;
    if (daisyLifes <= 0) {
      gameOver = true;
      gameOverSprite = new LGFX_Sprite();
      gameOverSprite->createSprite(99, 16);
      gameOverSprite->setSwapBytes(true);
      gameOverSprite->pushImage(0, 0, gameOverSprite->width(), gameOverSprite->height(), game_over);
      daisyDelay = millis() + 2000;
      return;
    }
    daisyMode = REBIRTH;
    daisyDelay = millis() + 1500;
    daisy->setPos(Point(lastDaisyPos.x, lastDaisyPos.y));
    if (eggs < 2) eggs = 2;
  }

  void daisyFlying() {
    if (daisy->getStatus() == VANISHED) return;
    if (daisy->getStatus() == NORMAL) daisy->drawOnSprite(background);
    daisy->onTick();
    if (daisy->getStatus() != VANISHED) {
      if (scroller->getCollisionIdx(BULLET, daisy) >= 0) {
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
        if (eggs > 99) eggs = 99;
        waitCorn = 22;  // TODO: this is hacky
      }
      waitCorn--;
      if (daisyWithFenceDelay > 0) {
        daisyWithFenceDelay--;
        if (daisyWithFenceDelay == 0) daisy->setUsrFlag0(true);
      }
    } else {
      daisyDx = 0;
      daisyDy = 0;
    }
  }

  void daisyWasShot() {
    daisyMode = IN_PEACES;
    daisyInPeaces->onTick();
    daisyInPeaces->drawAllOnBackground(background);
    if (daisyInPeaces->isReady() && (daisyInPeaces != NULL)) {
      delete daisyInPeaces;
      daisyDelay = millis() + 1000;
      backToLife();
      daisy->setPos(Point(0xffff, 0));
      scroller->setDaisyPos(daisy->getPos(), daisyMode);
    }
  }

  void daisyNextTry() {
    if (isGlobal(0xf)) {
      if (millis() > daisyDelay) {
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
    if (idx >= 0 && (scroller->getAbstractSprite(idx)->getStatus() != COLLIDED)) {
      scroller->getAbstractSprite(idx)->setStatus(COLLIDED);
      scroller->getAbstractSprite(idx)->setAnimCnt(anim);
      score + 4;
      return true;
    }
    return false;
  }

  void handleDaisyInput() {
    inputController->processed();
    inputController->poll();
    if (daisyMode == FLYING) {
      lastDaisyPos = Point(daisy->getPos().x, daisy->getPos().y);
    }
  }

  void handleDaisyFence() {
    if (daisyFenceCnt > 0) daisyFenceCnt--;
    else if (daisyFenceCnt == 0) {
      daisyFenceCnt--;
      daisy->setUsrFlag0(false);
      daisy->setAnimCnt(daisy->getAnimCnt() - 3);
    }
  }

  void handleDaisyEggs() {
    for (int n = 0; n < MAX_EGGS; n++) {
      if (eggSprite[n] != NULL) {
        if (eggSprite[n]->getStatus() == VANISHED) {
          eggSprite[n] = NULL;
        } else {
          bool eggHit = false;
          eggSprite[n]->onTick();
          eggHit |= checkEggHit(DOG, n, 6);
          eggHit |= checkEggHit(HUNTER, n, 6);
          if (eggHit) {
            eggSprite[n] = NULL;
          }
        }
      }
    }
  }

  void initAll() {
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
  }

public:
  GameMode(LGFX* _lcd, LGFX_Sprite* _background, InputController* _inputController, Scroller* _scroller)
    : AbstractMode(_lcd, _background, _inputController, _scroller) {
    currentMode = IN_GAME;
    daisy = new Daisy();
    life = new Life();
    eggsStyled = new StyledNumber(lcd, 2);
    scoreStyled = new StyledNumber(lcd, 5);
    initAll();
    for (int n = 0; n < MAX_EGGS; n++) eggSprite[n] = NULL;
  }

  ~GameMode() {
    
  }

  void killMode() {
    if (daisy != NULL) delete daisy;
    if (daisyInPeaces != NULL) delete daisyInPeaces;
    if (life != NULL) delete life;
    if (gameOverSprite != NULL) delete gameOverSprite;
    if (eggsStyled != NULL) delete eggsStyled;
    if (scoreStyled != NULL) delete scoreStyled;
  }

  void onTick() {
    if (!initialized) {
      restoreBg();
      initialized = true;
    } else if (gameOver) {
      daisy->setStatus(VANISHED);
      clearPlayfield();
      scroller->onTick();
      drawEggs();
      if ((globalCnt & 0xf) > 6) gameOverSprite->pushSprite(background, (background->width() >> 1) - (gameOverSprite->width() >> 1), (background->height() >> 1) - (gameOverSprite->height() >> 1) - 24, 0x0000);
      drawPlayfield();
      if (millis() > daisyDelay) {
        inputController->processed();
        inputController->poll();
        if (inputController->isTouched()) {
          currentMode = MENU;
          daisyDelay = millis() + 1500;
        }
      }
    } else {
      clearPlayfield();
      scroller->onTick();
      if (daisyMode == FLYING || daisyMode == PROTECTED) {
        if (daisyMode == PROTECTED) daisy->setStatus((isGlobal(2) ? NORMAL : INVISIBLE));
        daisyFlying();
      } else if (daisyMode == HIT_BY_BULLET) {
        daisyAway();
        daisyWasShot();
      } else if (daisyMode == IN_PEACES) {
        daisyWasShot();
      } else if (daisyMode == MILL_CRASH) {
        backToLife();
      } else if (daisyMode == ATE_BY_DOG) {
        backToLife();
      } else if (daisyMode == WOLF_CRASH) {
        backToLife();
      } else if (daisyMode == GOT_FENCE) {
        daisy->setAnimCnt(daisy->getAnimCnt() + 3);
        daisyMode = FLYING;
      } else if (daisyMode == WOLF_CRASH) {
        backToLife();
      } else if (daisyMode == REBIRTH) {
        daisyNextTry();
      }
      if (daisyMode == PROTECTED && millis() > daisyDelay) {
        daisy->setStatus(NORMAL);
        daisyMode = FLYING;
      }
      for (int n = 0; n < MAX_EGGS; n++)
        if (eggSprite[n] != NULL) eggSprite[n]->drawOnSprite(background);
      handleDaisy();
      drawPlayfield();
      drawEggs();
      drawLifes();
      drawScore();
      handleDaisyInput();
      handleDaisyFence();
      handleDaisyEggs();
      score++;
      if (survived > SCORE_FOR_SURVINING) {
        survived = 0;
        score++;
        if (isGlobal(0x1f)) Serial.printf("survived: %d, score: %d\n", survived, score);
      }
    }
    globalCnt++;
  }
};

#endif