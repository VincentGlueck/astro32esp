#ifndef _SPRITES_CPP_
#define _SPRITES_CPP_

#include "AbstractSprite.h"
#include "daisy.h"
#include "sprites_ground.h"

class Daisy : public AbstractSprite {
public:
  Daisy() : AbstractSprite("Daisy", 1 ) {
    addSprite(SingleSprite(Dimension(121, 108), (short unsigned int*)daisy_raw));
    keepInMemory = true;
  }

  void onTick() {
    pos.x++;
    if(pos.x > 310) pos.x = -120;
  }

};

class Mill : public AbstractSprite {
public:  
  Mill() : AbstractSprite("Mill", 4) {
    addSprite(SingleSprite(Dimension(24, 70), (short unsigned int*)mill01));
    addSprite(SingleSprite(Dimension(24, 70), (short unsigned int*)mill02));
    addSprite(SingleSprite(Dimension(24, 70), (short unsigned int*)mill03, Point(0,3)));
    addSprite(SingleSprite(Dimension(24, 70), (short unsigned int*)mill04));
    pos.x = 300;
    pos.y = 86;
    status = NORMAL;
  }

  void onTick() {
    tick++;
    if((tick & 1) == 1) {
      animCnt++;
      if(animCnt > 3) animCnt = 0;
    }
    if(status == NORMAL) {
      //Serial.printf("anim: %d\n", animCnt);
      //delay(1000);
      pos.x--;
      if(pos.x < -20) {
        status = READY;
        pos.x = 0xffff;
      }
    }
  }
};

#endif
