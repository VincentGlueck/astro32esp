#ifndef _ABSTRACT_SPRITE_H
#define _ABSTRACT_SPRITE_H

#include "lgfx_user.h"

enum Status {
  NORMAL,
  READY,
  COLLIDED,
  VANISHED
};

struct Point {
  int x;
  int y;
};

struct Dimension {
  int width;
  int height;
};

struct SingleSprite {
  Dimension dimension;
  unsigned short* ptr;
  Point delta = Point(0,0);
};

class AbstractSprite {
public:

  AbstractSprite(String _name, uint8_t _animations);
  ~AbstractSprite();
  
  Point getPos();
  void setPos(Point _pos);
  Dimension getDimension(uint8_t whichAnim);
  void drawOnSprite(LGFX_Sprite* background);
  void addSprite(SingleSprite _sprite);
  bool isLoaded();
  void setKeepInMemory(bool _keep);
  uint8_t rnd();
  uint8_t getAnimCnt();
  Status getStatus();
  void setStatus(Status _status);
  
  virtual void onTick() {};


protected:
  uint8_t animCnt;
  uint8_t currentAnimLoaded;
  bool keepInMemory;
  uint8_t loadedAnims;
  uint8_t animations;
  uint16_t tick;
  Point pos;
  String name;
  LGFX_Sprite lgfxSprite;
  SingleSprite* sprites;
  Status status;
  uint8_t za, zb, zc, zx;

};

#endif