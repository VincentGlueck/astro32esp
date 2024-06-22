#ifndef _SPRITES_HPP_
#define _SPRITES_HPP_

#include "AbstractSprite.h"
#include "sprites_ground.h"
#include "FastRandom.hpp"

class DaisyAwareSprite : public AbstractSprite {
protected:
  Point daisyPos;  
public:
  DaisyAwareSprite(String _name, uint8_t _animations) : AbstractSprite(_name, _animations) {}
  void setDaisyPos(Point _p) {
    daisyPos = Point(_p.x, _p.y);
  }
};

class BigDaisy : public AbstractSprite {
public:
  BigDaisy() : AbstractSprite("Daisy", 1 ) {
    addSprite(SingleSprite(Dimension(121, 108), (short unsigned int*)big_daisy));
    keepInMemory = true;
    usr_dx = 8;
    usr_ddx = -3;
    usr_flag0 = true;
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
    status = ((tick >> 2) & 3) < 1 ? VANISHED : NORMAL;
  }
};

class Mill : public AbstractSprite {
public:  
  Mill() : AbstractSprite("Mill", 4, 2) {
    addSprite(SingleSprite(Dimension(24, 70), (short unsigned int*)mill01));
    addSprite(SingleSprite(Dimension(24, 70), (short unsigned int*)mill02));
    addSprite(SingleSprite(Dimension(24, 70), (short unsigned int*)mill03, Point(0,3)));
    addSprite(SingleSprite(Dimension(24, 70), (short unsigned int*)mill04));
    pos.x = 305;
    pos.y = 86;
  }

  void onTick() {
    tick++;
    if((tick & 1) == 1) {
      animCnt++;
      if(animCnt > 3) animCnt = 0;
    }
    if(status == NORMAL) {
      pos.x--;
      if(pos.x < -19) {
        status = VANISHED;
        pos.x = 0xffff;
      }
    }
  }
};

class Fence : public AbstractSprite {
public:
  Fence() : AbstractSprite("Fence", 2) {
    addSprite(SingleSprite(Dimension(41, 48), (short unsigned int*)fence02));
    addSprite(SingleSprite(Dimension(41, 48), (short unsigned int*)fence01));
    pos.x = 305;
    pos.y = 108;
  }

  void onTick() {
    if(status == NORMAL) {
      animCnt = COLLIDED ? 1 : 0;
      pos.x--;
    }
    if(pos.x < -52) {
      status = VANISHED;
      pos.x = 0xffff;
    }
  }
};

class Corn : public AbstractSprite {
public:
  Corn() : AbstractSprite("Corn", 2, 2) {
    addSprite(SingleSprite(Dimension(13, 28), (short unsigned int*)corn01));
    addSprite(SingleSprite(Dimension(13, 20), (short unsigned int*)corn02));
    pos.y = 125;
  }

  void onTick() {
    pos.x--;
    if(pos.x < -9) {
      status = VANISHED;
      pos.x = 0xffff;
    }
  }
};

class Mountain : public AbstractSprite {
public:
  Mountain() : AbstractSprite("Mountain", 1, 1) {
    addSprite(SingleSprite(Dimension(45, 14), (short unsigned int*)mountain_raw));
    pos.y = 136;
    pos.x = 305;
  }  

  void onTick() {
    pos.x--;
    if(pos.x < -40) {
      status = VANISHED;
      pos.x = 0xffff;
    }
  }
};

class Dog : public DaisyAwareSprite {
public:
  Dog() : DaisyAwareSprite("Dog", 9) {
    addSprite(SingleSprite(Dimension(27, 21), (short unsigned int*)dog01));
    addSprite(SingleSprite(Dimension(28, 25), (short unsigned int*)dog02));
    addSprite(SingleSprite(Dimension(27, 29), (short unsigned int*)dog03));
    addSprite(SingleSprite(Dimension(28, 28), (short unsigned int*)dog04));
    addSprite(SingleSprite(Dimension(33, 29), (short unsigned int*)dog05));
    addSprite(SingleSprite(Dimension(25, 21), (short unsigned int*)dog06));
    addSprite(SingleSprite(Dimension(32, 29), (short unsigned int*)dog07));
    addSprite(SingleSprite(Dimension(32, 24), (short unsigned int*)dog08));
    addSprite(SingleSprite(Dimension(35, 24), (short unsigned int*)dog09));
    pos.y = 131;
    pos.x = 305;
  }

  void onTick() {
    pos.x--;
    if(pos.x < -40) {
      status = VANISHED;
      pos.x = 0xffff;
    }
  }
};

class Hunter : public DaisyAwareSprite {
public:
  Hunter() : DaisyAwareSprite("Hunter", 9) {
    addSprite(SingleSprite(Dimension(28, 31), (short unsigned int*)man01));
    addSprite(SingleSprite(Dimension(21, 35), (short unsigned int*)man02));
    addSprite(SingleSprite(Dimension(18, 37), (short unsigned int*)man03));
    addSprite(SingleSprite(Dimension(19, 37), (short unsigned int*)man04));
    addSprite(SingleSprite(Dimension(21, 35), (short unsigned int*)man05));
    addSprite(SingleSprite(Dimension(29, 31), (short unsigned int*)man06));
    addSprite(SingleSprite(Dimension(25, 32), (short unsigned int*)man07));
    addSprite(SingleSprite(Dimension(31, 37), (short unsigned int*)man08));
    addSprite(SingleSprite(Dimension(44, 11), (short unsigned int*)man09));
    pos.y = 131;
    pos.x = 305;
  }  

  void onTick() {
    pos.x--;
    if(pos.x < -40) {
      status = VANISHED;
      pos.x = 0xffff;
    }
  }
};

#endif
