#include "AbstractSprite.h"

AbstractSprite::AbstractSprite(String _name, uint8_t _animations, uint8_t _zPrio) {
  name = _name;
  animations = _animations;
  tick = 0;
  loadedAnims = 0;
  keepInMemory = false;
  loaded = false;
  animCnt = 0;
  zPrio = _zPrio;
  status = NORMAL;
  sprites = new SingleSprite[animations];
}

AbstractSprite::~AbstractSprite() {
  if(keepInMemory && loaded) lgfxSprite.deleteSprite();
  delete[] sprites;
}

Point AbstractSprite::getPos() {
  return pos;
}

void AbstractSprite::setKeepInMemory(bool _keep) {
  keepInMemory = true;
}

void AbstractSprite::setPos(Point _pos) {
  pos = _pos;
}

void AbstractSprite::addSprite(SingleSprite _sprite) {
  if(loadedAnims < animations) {
    sprites[loadedAnims++] = _sprite;
  } else {
    Serial.printf("Attempt to load more than %d sprites for %s!\n", animations, name);
  }
}

void AbstractSprite::drawOnSprite(LGFX_Sprite* background) {
  bool outOfMemory = false;
  if(loadedAnims < animations) {
    Serial.printf("FATAL, sprite '%s' has only loaded %d of %d!", name, loadedAnims, animations);
    sleep(10000);
  }
  if((status == VANISHED) || (pos.x == 0xffff)) return; // sprite is gone or out of visible range
  if(!keepInMemory || !loaded) {
    if(lgfxSprite.createSprite(sprites[animCnt].dimension.width, sprites[animCnt].dimension.height)) {
      lgfxSprite.setSwapBytes(true);
      lgfxSprite.pushImage(0, 0, sprites[animCnt].dimension.width, sprites[animCnt].dimension.height, &sprites[animCnt].ptr[0]);
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

String AbstractSprite::getName() {
  return name;
}


