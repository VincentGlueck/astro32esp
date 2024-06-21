#ifndef _SCROLLER_H_
#define _SCROLLER_H_

#include "sprites.hpp"

#define MAX_GROUND_SPRITES 20
#define MIN_NEXT_TICKS 15
#define MIN_NEXT_MILL 40
#define MIN_NEXT_FENCE 90
#define MIN_NEXT_CORN 130
//#define MIN_NEXT_DECORATION 45

class Scroller {

public:
  Scroller(LGFX_Sprite _background);
  ~Scroller();
  void onTick();

private:
  LGFX_Sprite background; 
  AbstractSprite* sprites[MAX_GROUND_SPRITES];
  uint8_t addGroundObject();
  uint8_t getFreeSlot();
  uint8_t waitTicks = MIN_NEXT_TICKS;
  uint8_t waitMill = MIN_NEXT_MILL;
  uint8_t waitFence = MIN_NEXT_FENCE;
  uint8_t waitCorn = MIN_NEXT_CORN;
  //uint8_t waitDecoration = MIN_NEXT_DECORATION;
  uint8_t createMill(uint8_t idx);
  uint8_t createFence(uint8_t idx);
  uint8_t createCorn(uint8_t idx);
  //uint8_t createDecoration(uint8_t idx);

};

#endif