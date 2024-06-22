#ifndef _ABSTRACT_SPRITE_H
#define _ABSTRACT_SPRITE_H

#include "lgfx_user.h"
#include "FastRandom.hpp"

#define PLAYFIELD_WIDTH 310
#define PLAYFIELD_HEIGHT 176

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

  AbstractSprite(String _name, uint8_t _animations = 1, uint8_t _zPro = 0);
  AbstractSprite();
  ~AbstractSprite();

  virtual void onTick() {};

  Point getPos();
  void setPos(Point _pos);
  Dimension getDimension(uint8_t whichAnim);
  void drawOnSprite(LGFX_Sprite* background);
  void addSprite(SingleSprite _sprite);
  bool isLoaded();
  void setKeepInMemory(bool _keep);
  uint8_t getAnimCnt();
  Status getStatus();
  void setStatus(Status _status);
  uint8_t getZPrio();
  void setZPrio(uint8_t _prio);
  String getName();
 



protected:
  uint8_t animCnt;
  bool keepInMemory;
  bool loaded = false;
  uint8_t loadedAnims;
  uint8_t animations = 0;
  uint8_t zPrio = 0;
  uint16_t tick;
  Point pos;
  String name;
  LGFX_Sprite lgfxSprite;
  SingleSprite* sprites;
  Status status;
  int usr_dx, usr_ddx, usr_dy, usr_ddy, usr_a, usr_b, usr_c;
  bool usr_flag0, usr_flag1;

};

#endif