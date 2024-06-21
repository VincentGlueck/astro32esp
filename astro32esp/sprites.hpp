#ifndef _SPRITES_HPP_
#define _SPRITES_HPP_

#include "AbstractSprite.h"
#include "daisy.h"
#include "sprites_ground.h"
#include "FastRandom.hpp"

class Daisy : public AbstractSprite {
public:
  Daisy() : AbstractSprite("Daisy", 1 ) {
    addSprite(SingleSprite(Dimension(121, 108), (short unsigned int*)daisy_raw));
    keepInMemory = true;
    usr_dx = 8;
    usr_ddx = -3;
    usr_flag0 = true;
    status = NORMAL;
  }

  void onTick() {
    tick++;
    if(usr_flag0) {
      pos.x += usr_dx;
      if((tick & 0x1f) == 0x1f) {
        usr_dx += usr_ddx;
        if(usr_dx < 1) usr_dx = 1;
      }
      if(((tick & 0x3f) == 0x3f) && (usr_ddx < 0)) usr_ddx++;
      if((pos.x+65) > (PLAYFIELD_WIDTH >> 1)) {
        pos.x = (PLAYFIELD_WIDTH >> 1) - 65;
        usr_a = pos.y;
        usr_flag0 = false;
      }
    } else if ((tick & 1) == 1) {
      pos.x = (PLAYFIELD_WIDTH >> 1) - 62 + rnd(3);
      pos.y = usr_a - 2 + rnd(3);
    }
  }

};

class GetReady : public AbstractSprite {
public:  
  GetReady() : AbstractSprite("gr", 1) {
    addSprite(SingleSprite(Dimension(148, 18), (short unsigned int*)get_ready));
    keepInMemory = true;
    pos.x = 80;
    pos.y = 108;
    usr_dx = 1;
    status = NORMAL;
  }

  void onTick() {
    tick++;
    if((tick & 7) == 7) {
      pos.x += usr_dx;
      if(pos.x > 100) {
        pos.x = 100;
        usr_dx = -1;
      } else if (pos.x < 60) {
        pos.x = 60;
        usr_dx = 1;
      }
    }
    pos.y = 107 + rnd(3);
    status = ((tick >> 2) & 3) < 1 ? READY : NORMAL;
  }
};

class Mill : public AbstractSprite {
public:  
  Mill() : AbstractSprite("Mill", 4) {
    addSprite(SingleSprite(Dimension(24, 70), (short unsigned int*)mill01));
    addSprite(SingleSprite(Dimension(24, 70), (short unsigned int*)mill02));
    addSprite(SingleSprite(Dimension(24, 70), (short unsigned int*)mill03, Point(0,3)));
    addSprite(SingleSprite(Dimension(24, 70), (short unsigned int*)mill04));
    pos.x = 305;
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
      if(pos.x < -19) {
        status = READY;
        pos.x = 0xffff;
      }
    }
  }
};

class Fence : public AbstractSprite {
public:
  Fence() : AbstractSprite("Fence", 2) {
    addSprite(SingleSprite(Dimension(21, 24), (short unsigned int*)fence02));
    addSprite(SingleSprite(Dimension(21, 24), (short unsigned int*)fence01));
    pos.x = 305;
    pos.y = 127;
    status = NORMAL;
  }

  void onTick() {
    if(status == NORMAL) {
      animCnt = COLLIDED ? 1 : 0;
      pos.x--;
      if(pos.x < -15) {
        status = READY;
        pos.x = 0xffff;
      }
    }
  }
};

class Corn : public AbstractSprite {
public:
  Corn() : AbstractSprite("Corn", 2) {
    addSprite(SingleSprite(Dimension(8, 16), (short unsigned int*)corn01));
    addSprite(SingleSprite(Dimension(10, 16), (short unsigned int*)corn02));
    status = NORMAL;
    pos.y = 136;
  }

  void onTick() {
    pos.x--;
    if(pos.x < -3) {
      status = READY;
      pos.x = 0xffff;
    }
  }
};

/*
class Decoration : public AbstractSprite {
public:
  Decoration() : AbstractSprite("deco", 2) {
    addSprite(SingleSprite(Dimension(26, 10), (short unsigned int*)stone));
    addSprite(SingleSprite(Dimension(12, 16), (short unsigned int*)gras));
    //addSprite(SingleSprite(Dimension(45, 14), (short unsigned int*)mountain));
    pos.x = 305;
    int which = rnd(1);
    //if(which == 2) which = 1;
    animCnt = which;
    if(which == 0) pos.y = 145; else pos.y = 142;
  }

  void onTick() {
    tick++;
    pos.x--;
    if((tick & 1) == 1) pos.x--;
    if(pos.x < -9) {
      status = READY;
      pos.x = 0xffff;
    }
  }
};
*/

#endif
