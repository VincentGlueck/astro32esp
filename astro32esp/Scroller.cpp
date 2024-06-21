#include "Scroller.h"

Scroller::Scroller(LGFX_Sprite _background) {
  background = _background;
  for(int n=0; n<MAX_GROUND_SPRITES; n++) sprites[n] = NULL;
}

Scroller::~Scroller() {
}

uint8_t Scroller::getFreeSlot() {
  int n=0;
  while(n < MAX_GROUND_SPRITES) {
    if(sprites[n] == NULL) {
      return n;
    }
    n++;
  }
  Serial.println("Out of slots!");
  return -1;
}

uint8_t Scroller::createMill(uint8_t idx) {
  if(waitMill > 0) return 0;
  waitMill = MIN_NEXT_MILL;
  waitFence += 30;
  sprites[idx] = new Mill();
  return 5;
}

uint8_t Scroller::createFence(uint8_t idx) {
  if(waitFence > 0) {
    return 0;
  }
  waitFence = MIN_NEXT_FENCE;
  waitMill += 20;
  sprites[idx] = new Fence();
  return 10;
}

uint8_t Scroller::createCorn(uint8_t idx) {
  sprites[idx] = new Corn();
  return -1;
}

uint8_t Scroller::addGroundObject() {
  int idx = getFreeSlot();
  if(idx == -1) {
    Serial.println("No free slot!");
    return 0;
  }
  int what = rnd(7);
  switch(what) {
    case 0: return createMill(idx);
    case 1: return createMill(idx);
    case 3: return createMill(idx);
    case 4: return createFence(idx);
    case 5: return 10;
    case 6: {
      if(waitCorn > 0) return 0;
      int x = 315;
      for(int n=0; n<3+rnd(3); n++) {
        createCorn(idx);
        sprites[idx]->setPos(Point(x, sprites[idx]->getPos().y));
        x += 20;
        idx = getFreeSlot();
        if(idx == -1) break;
      }
      waitCorn = MIN_NEXT_CORN;
      waitMill = 15;
      if(waitFence < MIN_NEXT_FENCE) waitFence += 10;
      return 5;
    }
    case 7: break; // MAX currently
    default: break;
  }
  return 0;
}

void Scroller::onTick() {
  if(waitTicks > 0) waitTicks--;
  if((waitTicks <= 0) && (rnd(1) == 1)) {
    if(waitMill > 0) waitMill--;
    if(waitFence > 0) waitFence--;
    if(waitCorn > 0) waitCorn--;
    Serial.printf("wait: fence:%d, mill:%d, corn:%d\n", waitFence, waitFence, waitCorn);
    waitTicks = addGroundObject();
  }
  for(int n=0; n<MAX_GROUND_SPRITES; n++) {
    if(sprites[n] != NULL) {
      if(sprites[n]->getStatus() == READY) {
        delete sprites[n];
        sprites[n] = NULL;
      } else {
        sprites[n]->drawOnSprite(&background);
        sprites[n]->onTick();
      }
    }
  }
}

