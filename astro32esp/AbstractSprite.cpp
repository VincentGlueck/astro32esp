#include "AbstractSprite.h"

AbstractSprite::AbstractSprite(String _name, uint8_t _animations = 1) {
  name = _name;
  animations = _animations;
  tick = 0;
  loadedAnims = 0;
  keepInMemory = false;
  animCnt = 0;
  status = NORMAL;
  sprites = new SingleSprite[animations];
}

AbstractSprite::~AbstractSprite() {
  if(keepInMemory) lgfxSprite.deleteSprite();
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
  if(loadedAnims < animations) sprites[loadedAnims++] = _sprite;
}

void AbstractSprite::drawOnSprite(LGFX_Sprite* background) {
  bool outOfMemory = false;
  if(loadedAnims < animations) {
    Serial.printf("FATAL, sprite '%s' has only loaded %d of %d!", name, loadedAnims, animations);
    sleep(10000);
  }
  if(status == READY) return; // sprite is waiting
  if(!keepInMemory) {
    if(lgfxSprite.createSprite(sprites[animCnt].dimension.width, sprites[animCnt].dimension.height)) {
      lgfxSprite.setSwapBytes(true);
      lgfxSprite.pushImage(sprites[animCnt].delta.x, sprites[animCnt].delta.y, sprites[animCnt].dimension.width, sprites[animCnt].dimension.height, &sprites[animCnt].ptr[0]);
    } else {
      Serial.println("AbstractSprite::Out of memory");
      outOfMemory = true;
    }
  }
  if(!outOfMemory) lgfxSprite.pushSprite(background, pos.x, pos.y, 0x0000);
  if(!keepInMemory) lgfxSprite.deleteSprite();
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



