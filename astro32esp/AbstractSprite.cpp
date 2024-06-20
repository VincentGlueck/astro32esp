#include "AbstractSprite.h"

AbstractSprite::AbstractSprite(String _name, uint8_t _animations = 1) {
  name = _name;
  animations = _animations;
  tick = 0;
  loadedAnims = 0;
  currentAnimLoaded = 0xff;
  keepInMemory = false;
  animCnt = 0;
  status = NORMAL;
  za = random(256);
  zb = random(256);
  zc = random(256);
  zx = random(256);
  sprites = new SingleSprite[animations];
}

AbstractSprite::~AbstractSprite() {
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
  if(loadedAnims < animations) {
    Serial.printf("FATAL, sprite '%s' has only loaded %d of %d!", name, loadedAnims, animations);
    sleep(10000);
  }
  if(status == READY) return; // sprite is waiting
  if(!keepInMemory || (currentAnimLoaded != animCnt)) {
    if(lgfxSprite.createSprite(sprites[animCnt].dimension.width, sprites[animCnt].dimension.height)) {
      lgfxSprite.setSwapBytes(true);
      lgfxSprite.pushImage(sprites[animCnt].delta.x, sprites[animCnt].delta.y, sprites[animCnt].dimension.width, sprites[animCnt].dimension.height, &sprites[animCnt].ptr[0]);
    } else {
      Serial.println("Out of memory!");
      sleep(10000);
    }
  }
  lgfxSprite.pushSprite(background, pos.x, pos.y, 0x0000);
  if(keepInMemory) {
    currentAnimLoaded = animCnt;
  } else {
    lgfxSprite.deleteSprite();
    currentAnimLoaded = 0xff;
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

uint8_t AbstractSprite::rnd() {
  zx++;
  za = (za ^ zc ^ zx);
  zb = (zb + za);
  zc = ((zc + (zb >> 1)) ^ za);
  return zc;
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



