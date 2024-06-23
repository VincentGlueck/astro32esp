#include "AbstractSprite.h"

//#define DEBUG_FRAME

AbstractSprite::AbstractSprite(int _type, uint8_t _animations, uint8_t _zPrio) {
  type = _type;
  animations = _animations;
  zPrio = _zPrio;
  tick = 0;
  loadedAnims = 0;
  keepInMemory = false;
  loaded = false;
  animCnt = 0;
  hit = false;
  status = NORMAL;
  sprites = new SingleSprite[animations];
}

AbstractSprite::~AbstractSprite() {
  if(!keepInMemory && loaded) lgfxSprite.deleteSprite();
  delete[] sprites;
}

Point AbstractSprite::getPos() {
  return pos;
}

bool AbstractSprite::isCollided(AbstractSprite* other) {
  return false;
}

void AbstractSprite::setDaisyPos(Point _p) {
  daisyPos = Point(_p.x, _p.y);
}

void AbstractSprite::setEggPos(Point _p) {
  eggPos = Point(_p.x, _p.y);
}

void AbstractSprite::setKeepInMemory(bool _keep) {
  keepInMemory = true;
}

void AbstractSprite::setPos(Point _pos) {
  pos = _pos;
}

void AbstractSprite::setSubSprite(AbstractSprite* _sprite) {
  subSprite = _sprite;
}

AbstractSprite* AbstractSprite::getSubSprite() {
  return subSprite;
}

void AbstractSprite::setUsrDxDy(int dx, int dy, int ddx, int ddy) {
  usr_dx = dx;
  usr_dy = dy;
  usr_ddx = ddx;
  usr_ddy = ddy;
}

void AbstractSprite::setUsrAbc(int a, int b, int c) {
  usr_a = a;
  usr_b = b;
  usr_c = c;
}

void AbstractSprite::addSprite(SingleSprite _sprite) {
  if(loadedAnims < animations) {
    sprites[loadedAnims++] = _sprite;
  } else {
    Serial.printf("Attempt to load more than %d sprites for %d!\n", animations, type);
  }
}

void AbstractSprite::setAnimCnt(uint8_t _cnt) {
  if(_cnt < animations) animCnt = _cnt; else Serial.printf("%d does not have %d frames (%d)!\n", type, _cnt, animations);
}

void AbstractSprite::setUsrFlag0(bool _flag) {
  usr_flag0 = _flag;
}

void AbstractSprite::drawOnSprite(LGFX_Sprite* background) {
  bool outOfMemory = false;
  if(loadedAnims < animations) {
    Serial.printf("FATAL, sprite '%d' has only loaded %d of %d!", type, loadedAnims, animations);
    sleep(10000);
  }
  if((status == VANISHED) || (pos.x == 0xffff)) return; // sprite is gone or out of visible range
  if(!keepInMemory || !loaded) {
    if(lgfxSprite.createSprite(sprites[animCnt].dimension.width, sprites[animCnt].dimension.height)) {
      lgfxSprite.setSwapBytes(true);
      lgfxSprite.pushImage(0, 0, sprites[animCnt].dimension.width, sprites[animCnt].dimension.height, &sprites[animCnt].ptr[0]);
#ifdef DEBUG_FRAME
      lgfxSprite.drawRect(0, 0, sprites[animCnt].dimension.width, sprites[animCnt].dimension.height, TFT_DARKCYAN);
#endif      
      loaded = true;
    } else {
      Serial.println("AbstractSprite::Out of memory");
      outOfMemory = true;
    }
  }
  if(!outOfMemory) {
    lgfxSprite.pushSprite(background, pos.x + sprites[animCnt].delta.x, pos.y + sprites[animCnt].delta.y, 0x0000);
    if(!keepInMemory) lgfxSprite.deleteSprite();
  }
}

bool AbstractSprite::isHit() {
  return hit;
}

void AbstractSprite::setHit() {
  hit = true;
}

bool AbstractSprite::isLoaded() {
  return loadedAnims >= animations;
}

Dimension AbstractSprite::getDimension(uint8_t whichAnim) {
  if(whichAnim < loadedAnims) {
    return sprites[whichAnim].dimension;
  }
  return Dimension(-1, -1);
}

Status AbstractSprite::getStatus() {
  return status;
}

void AbstractSprite::setStatus(Status _status) {
  status = _status;
}

uint8_t AbstractSprite::getAnimCnt() {
  return animCnt;
}

uint8_t AbstractSprite::getZPrio() {
  return zPrio;
}

void AbstractSprite::setZPrio(uint8_t _prio) {
  zPrio = _prio;
}

int AbstractSprite::getType() {
  return type;
}


