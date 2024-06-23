#ifndef _SCROLLER_H_
#define _SCROLLER_H_

#include "sprites.hpp"

#define MAX_GROUND_SPRITES 32
#define MIN_NEXT_TICKS 10
#define MIN_NEXT_MILL 12
#define MIN_NEXT_FENCE 15
#define MIN_NEXT_CORN 32
#define MIN_NEXT_MOUNTAIN 30
#define MIN_NEXT_DOG 38
#define MIN_NEXT_HUNTER 19
//#define MIN_NEXT_DECORATION 45

class Scroller {

public:
  Scroller(LGFX_Sprite _background);
  ~Scroller();
  void onTick();
  void setDifficulty(uint8_t _difficulty);
  void setSpeed(uint8_t _speed);
  void setDaisyPos(Point _p);
  void setEggPos(Point _p);
  bool isCollision(int type, AbstractSprite* sprite);

private:
  LGFX_Sprite background; 
  AbstractSprite* sprites[MAX_GROUND_SPRITES];
  void addGroundObject();
  void createMill(uint8_t idx);
  void createFence(uint8_t idx);
  void createCorn(uint8_t idx);
  void createMountain(uint8_t idx);
  void createDog(uint8_t idx);
  void createHunter(uint8_t idx);
  uint8_t getFreeSlot();
  bool isCollided(Point p0, Dimension d0, Point p1, Dimension d1);
  
  uint8_t difficulty = 0;
  uint8_t waitTicks = MIN_NEXT_TICKS;
  uint8_t waitMill = MIN_NEXT_MILL;
  uint8_t waitFence = MIN_NEXT_FENCE;
  uint8_t waitCorn = MIN_NEXT_CORN;
  uint8_t waitMountain = MIN_NEXT_MOUNTAIN;
  uint8_t waitDog = MIN_NEXT_DOG;
  uint8_t waitHunter = MIN_NEXT_HUNTER;
  uint8_t speed = 1;
  Point daisyPos;
  Point eggPos;
  
};

#endif