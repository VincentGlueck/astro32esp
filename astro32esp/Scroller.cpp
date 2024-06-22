#include "Scroller.h"

Scroller::Scroller(LGFX_Sprite _background) {
  background = _background;
  for(int n=0; n<MAX_GROUND_SPRITES; n++) sprites[n] = NULL;
}

Scroller::~Scroller() {
}

void Scroller::setDifficulty(uint8_t _difficulty) {
  difficulty = _difficulty;
}

void Scroller::setSpeed(uint8_t _speed) {
  speed = _speed;
}

void Scroller::setDaisyPos(Point _daisyPos) {
  daisyPos = _daisyPos;
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
  for(int n=0; n<MAX_GROUND_SPRITES; n++) {
    Serial.printf("n:%d :", n);
    if(sprites[n] != NULL) {
      Serial.printf("is a %s, pos.x: %d, status: %d\n", sprites[n]->getName(), sprites[n]->getPos().x, sprites[n]->getStatus());
    } else Serial.printf("NULL\n");
  }
  return -1;
}

void Scroller::createMill(uint8_t idx) {
  if(waitMill > 0) return;
  waitMill = MIN_NEXT_MILL + rnd(0xf);
  sprites[idx] = new Mill();
  if(waitFence < 30) waitFence += 30;
  if(waitDog < 15) waitDog += 15;
  if(waitHunter < 15) waitHunter += 15;
  waitMill = MIN_NEXT_MILL;
}

void Scroller::createFence(uint8_t idx) {
  if(waitFence > 0) return;
  sprites[idx] = new Fence();
  if(waitMill < 20) waitMill += 20;
  if(waitDog < 15) waitDog += 15;
  waitFence = MIN_NEXT_FENCE + rnd(0xf);
}

void Scroller::createCorn(uint8_t idx) {
  if(waitCorn > 0) return;
  int x = 315;
  for(int n=0; n<3+rnd(3); n++) {
    sprites[idx] = new Corn();
    sprites[idx]->setPos(Point(x, sprites[idx]->getPos().y));
    x += 21;
    waitMill += 2;
    idx = getFreeSlot();
    if(idx == -1) break;
  }
  waitCorn = MIN_NEXT_CORN + rnd(0xf);
}

void Scroller::createMountain(uint8_t idx) {
  if(waitMountain > 0) return;
  sprites[idx] = new Mountain();
  waitMountain = MIN_NEXT_MOUNTAIN + rnd(0xf);
}

void Scroller::createDog(uint8_t idx) {
  if(waitDog > 0) return;
  sprites[idx] = new Dog();
  waitDog = MIN_NEXT_DOG + rnd(0xf);
}

void Scroller::createHunter(uint8_t idx) {
  if(waitHunter > 0) return;
  sprites[idx] = new Hunter();
  waitHunter = MIN_NEXT_HUNTER + rnd(0xf);
  if(waitMill < 15) waitMill += 15;
  if(waitFence < 10) waitFence += 10;
}

void Scroller::addGroundObject() {
  int idx = getFreeSlot();
  if(idx == -1) {
    Serial.println("No free slot!");
    return;
  }
  waitTicks = MIN_NEXT_TICKS;
  int what = rnd(7);
  switch(what) {
    case 0: createMill(idx); break;
    case 1: createHunter(idx); break;
    case 3: break;
    case 4: createFence(idx); break;
    case 5: createMountain(idx); waitTicks = 5; break;
    case 6: createCorn(idx); break;
    case 7: createDog(idx); break; // MAX currently
    default: break;
  }
}


void Scroller::onTick() {
  if(waitTicks > 0) waitTicks--;
  for(int z=0; z<3; z++) {
    for(int n=0; n<MAX_GROUND_SPRITES; n++) {
      if(sprites[n] != NULL) {
        if((z == 2) && (sprites[n]->getStatus() == VANISHED)) {
          delete sprites[n];
          sprites[n] = NULL;
        } else if(z == sprites[n]->getZPrio()) {
          if(speed > 1) sprites[n]->setPos(Point(sprites[n]->getPos().x - (speed-1), sprites[n]->getPos().y));
          sprites[n]->drawOnSprite(&background);
          sprites[n]->onTick();
        }
      }
    }
  }
  if((waitTicks <= 0) && ((rnd(1) == 1) || (difficulty > 2))) {
    addGroundObject();
    if(waitMill > 0) waitMill--;
    if((waitMill > 0) && (difficulty > 1)) waitMill--;
    if(waitFence > 0) waitFence--;
    if(waitCorn > 0) waitCorn--;
    if(waitDog > 0) waitDog--;
    if(waitMountain > 0) waitMountain--;
    if(waitHunter > 0) waitHunter--;
    int f = 0;
    for(int n=0; n<MAX_GROUND_SPRITES; n++) if(sprites[n] == NULL) f++;
    //Serial.printf("wait: fence:%d, mill:%d, corn:%d, mountain:%d, dog:%d, hunter:%d, free slots:%d\n", waitFence, waitFence, waitCorn, waitMountain, waitDog, waitHunter, f);
  }

}

