#ifndef _SPRITES_HPP_
#define _SPRITES_HPP_

#include "AbstractSprite.h"
#include "sprites_raw.h"
#include "FastRandom.hpp"

#define HUNTER_SHOOTS

enum SpriteTypes {
  BIGDAISY,
  GETREADY,
  MOUNTAIN,
  MILL,
  FENCE,
  CORN,
  DOG,
  DAISY,
  BULLET,
  HUNTER,
  GRAS,
  EGG,
  WOLF,
  DAISY_IN_PEACES,
  LIFE,
  NUMBERS
};

class BigDaisy : public AbstractSprite {
public:
  BigDaisy() : AbstractSprite(BIGDAISY, 1) {
    addSprite(SingleSprite(Dimension(121, 108), (short unsigned int*)big_daisy));
    keepInMemory = true;
    usr_dx = 8;
    usr_ddx = -3;
    usr_flag0 = true;
    usr_flag1 = false;
  }

  void onTick() {
    tick++;
    if(usr_flag0 || usr_flag1) {
      pos.x += usr_dx;
      if((tick & 0x1f) == 0x1f) {
        usr_dx += usr_ddx;
        if(usr_dx < 1) usr_dx = 1;
      }
      if(!usr_flag1) {
        if(((tick & 0x3f) == 0x3f) && (usr_ddx < 0)) usr_ddx++;
        if((pos.x+65) > (PLAYFIELD_WIDTH >> 1)) {
          pos.x = (PLAYFIELD_WIDTH >> 1) - 65;
          usr_a = pos.y;
          usr_flag0 = false;
        }
      } else {
        pos.x + usr_ddx;
        usr_ddx +=2;
      }
    } else if ((tick & 1) == 1) {
      pos.x = (PLAYFIELD_WIDTH >> 1) - 62 + rnd(3);
      pos.y = usr_a - 2 + rnd(3);
      if(tick > 140) {
        usr_flag1 = true;
        usr_ddx = 1;
      }
    }
  }

};

class GetReady : public AbstractSprite {
public:  
  GetReady() : AbstractSprite(GETREADY, 1) {
    addSprite(SingleSprite(Dimension(148, 18), (short unsigned int*)get_ready));
    keepInMemory = true;
    pos.x = 80;
    pos.y = 58;
    usr_dy = pos.y;
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
    pos.y = usr_dy + rnd(3);
    if(((tick & 1) == 1) && (usr_dy < 98)) usr_dy++;
    status = ((tick >> 2) & 3) < 1 ? VANISHED : NORMAL;
  }
};

class Mill : public AbstractSprite {
public:  
  Mill() : AbstractSprite(MILL, 8) {
    addSprite(SingleSprite(Dimension(24, 70), (short unsigned int*)mill01)); // original mill03 removed (to avoid flickering); gfx was misplaced 23 year ago and cannot be fixed
    addSprite(SingleSprite(Dimension(24, 70), (short unsigned int*)mill02));
    addSprite(SingleSprite(Dimension(24, 70), (short unsigned int*)mill03));
    addSprite(SingleSprite(Dimension(34, 70), (short unsigned int*)millcrash01, Point(-10, 0)));
    addSprite(SingleSprite(Dimension(33, 70), (short unsigned int*)millcrash02, Point(-4, -1)));
    addSprite(SingleSprite(Dimension(33, 70), (short unsigned int*)millcrash03, Point(-4, -1)));
    addSprite(SingleSprite(Dimension(32, 70), (short unsigned int*)millcrash04, Point(-1, -1)));
    addSprite(SingleSprite(Dimension(33, 70), (short unsigned int*)millcrash05, Point(0, -1)));
    pos.x = 305;
    pos.y = 84;
    usr_flag0 = false;
  }

  void onTick() {
    tick++;
    if(status != VANISHED) {
      if(!usr_flag0 && (tick & 1) == 1) {
        animCnt++;
        if(animCnt > 2) animCnt = 0;
      } else if(usr_flag0 && ((tick & 3) == 3)) {
        animCnt++;
        if(animCnt >= animations) {
          animCnt = rnd(1);
          usr_flag0 = false;
        }
      }
      pos.x--;
      if(pos.x < -29) {
        status = VANISHED;
        pos.x = 0xffff;
      }
    }
  }
};

class Egg : public AbstractSprite {
public:
  Egg() : AbstractSprite(EGG, 11) {
    addSprite(SingleSprite(Dimension(9, 9), (short unsigned int*)egg01));
    addSprite(SingleSprite(Dimension(9, 9), (short unsigned int*)egg02));
    addSprite(SingleSprite(Dimension(9, 9), (short unsigned int*)egg03, Point(1,0)));
    addSprite(SingleSprite(Dimension(9, 9), (short unsigned int*)egg04, Point(1,0)));
    addSprite(SingleSprite(Dimension(9, 9), (short unsigned int*)egg05));
    addSprite(SingleSprite(Dimension(9, 9), (short unsigned int*)egg06));
    addSprite(SingleSprite(Dimension(9, 9), (short unsigned int*)egg07, Point(1,0)));

    addSprite(SingleSprite(Dimension(13, 8), (short unsigned int*)eggdamaged01));
    addSprite(SingleSprite(Dimension(14, 8), (short unsigned int*)eggdamaged02, Point(-1,0)));
    addSprite(SingleSprite(Dimension(14, 7), (short unsigned int*)eggdamaged03, Point(-1,1)));
    addSprite(SingleSprite(Dimension(14, 5), (short unsigned int*)eggdamaged04, Point(-1,2)));
    usr_ddy = 12;
    usr_ddx = rnd(0xf) + 35;
    usr_dx = 4096+1024;
    usr_dy = 38;
  }

  void onTick() {
    tick++;
    if(status == NORMAL) {
      if((pos.y > 143) && (animCnt < 7)) {
        pos.y = 143;
        animCnt = 7;
        status = COLLIDED; // means: on ground
      }
      if(animCnt >= 7) {
        pos.x--;
        if(pos.x < -12) {
          status = VANISHED;
          pos.x = 0xffff;
        }
        if(animCnt < (animations-1)) animCnt++;
      } else {
        //Serial.printf("pos.x: %d, pos.y: %d, ddx: %d, ddy: %d\n", pos.x, pos.y, usr_ddx, usr_ddy);
        animCnt++;
        if(usr_ddx > 128) usr_ddx -= 4;
        if((tick & 1) == 1) if(usr_ddy < 512) usr_ddy += 64;
        usr_dx += usr_ddx;
        usr_dy += usr_ddy;
        pos.x += (usr_dx >> 11);
        pos.y += (usr_dy >> 11);
        if(animCnt > 6) animCnt = 0;
      }
    } else {
      pos.x--;
    }     
  }
};

class Fence : public AbstractSprite {
public:
  Fence() : AbstractSprite(FENCE, 2, 2) {
    addSprite(SingleSprite(Dimension(41, 48), (short unsigned int*)fence02));
    addSprite(SingleSprite(Dimension(41, 48), (short unsigned int*)fence01));
    pos.x = 305;
    pos.y = 108;
  }

  void onTick() {
    if(status != VANISHED) {
      animCnt = 1;
      if((status == COLLIDED) && (daisyPos.x + 4) > pos.x) animCnt = 0;
      pos.x--;
      if(pos.x < -52) {
        status = VANISHED;
        pos.x = 0xffff;
      }
    } else Serial.println("Fence vanished");
  }
};

class Corn : public AbstractSprite {
public:
  Corn() : AbstractSprite(CORN, 2, 2) {
    addSprite(SingleSprite(Dimension(13, 28), (short unsigned int*)corn01));
    addSprite(SingleSprite(Dimension(13, 20), (short unsigned int*)corn02, Point(0, 8)));
    pos.y = 125;
  }

  void onTick() {
    pos.x--;
    animCnt = (status == COLLIDED) ? 1 : 0;
    if(pos.x < -9) {
      status = VANISHED;
      pos.x = 0xffff;
    }
  }
};

class Mountain : public AbstractSprite {
public:
  Mountain() : AbstractSprite(MOUNTAIN, 1, 2) {
    addSprite(SingleSprite(Dimension(71, 22), (short unsigned int*)mountain_raw));
    pos.y = 130+rnd(1);
    pos.x = 305;
  }  

  void onTick() {
    pos.x--;
    if(pos.x < -67) {
      status = VANISHED;
      pos.x = 0xffff;
    }
  }
};

class Dog : public AbstractSprite {
public:
  Dog() : AbstractSprite(DOG, 9) {
    addSprite(SingleSprite(Dimension(27, 21), (short unsigned int*)dog01, Point(0, 5)));
    addSprite(SingleSprite(Dimension(28, 25), (short unsigned int*)dog02, Point(0, -4)));
    addSprite(SingleSprite(Dimension(27, 29), (short unsigned int*)dog03, Point(0, -21)));
    addSprite(SingleSprite(Dimension(28, 28), (short unsigned int*)dog04, Point(0, 2)));

    addSprite(SingleSprite(Dimension(33, 29), (short unsigned int*)dog05, Point(0, -12))); // 4 - got daisy
    addSprite(SingleSprite(Dimension(35, 21), (short unsigned int*)dog06, Point(0, 5)));

    addSprite(SingleSprite(Dimension(32, 29), (short unsigned int*)dog07, Point(0, -4))); // 6 - hit by egg
    addSprite(SingleSprite(Dimension(32, 24), (short unsigned int*)dog08, Point(0, 2)));
    addSprite(SingleSprite(Dimension(33, 26), (short unsigned int*)dog09, Point(0, 5)));
    pos.y = 124;
    pos.x = 305;
    animCnt = 0;
    usr_flag0 = false; // jump
    usr_flag1 = false; // goddya
  }

  void onTick() {
    tick++;
    pos.x--;
    if(pos.x < -40) {
      status = VANISHED;
      pos.x = 0xffff;
    }
    if(status == COLLIDED) {
      if((tick & 7) == 7) {
        if(animCnt < (animations-1)) animCnt++;
      }
    } else {
      if(usr_flag0 && !usr_flag1 && ((tick & 0xf) == 0xf)) {
        animCnt++;
        if(animCnt > 3) {
          animCnt = 0;
          usr_flag0 = false;
        }
      } else if(usr_flag1 && ((tick & 0xf) == 0xf)) {
        if(animCnt < 5) animCnt++;
      } else if(!usr_flag0 && ((tick & 3) == 3) && (rnd(3) == 3)) {
        int dxDaisyX = pos.x - daisyPos.x;
        int dxDaisyY = 89-(abs(pos.y - daisyPos.y));
        if((abs(dxDaisyX) < 22)) {
          if(dxDaisyY > 50) {
            usr_flag0 = true;
            animCnt = 1;
          }
        }
      }
    }
  }
};

class Daisy : public AbstractSprite {
public:
  Daisy() : AbstractSprite(DAISY, 6) {
    addSprite(SingleSprite(Dimension(24, 24), (short unsigned int*)daisy01, Point(0, 0)));
    addSprite(SingleSprite(Dimension(24, 24), (short unsigned int*)daisy02, Point(0, 0)));
    addSprite(SingleSprite(Dimension(24, 24), (short unsigned int*)daisy03, Point(0, 0)));
    addSprite(SingleSprite(Dimension(24, 25), (short unsigned int*)daisy04, Point(0, 0)));
    addSprite(SingleSprite(Dimension(24, 24), (short unsigned int*)daisy05, Point(0, 0)));
    addSprite(SingleSprite(Dimension(24, 24), (short unsigned int*)daisy06, Point(0, 0)));
    pos.y = 55;
    pos.x = 25;
    usr_flag0 = false;
    usr_flag1 = false;
  }

  void onTick() {
    if(usr_flag0 != usr_flag1) {
      if(animCnt > 2) animCnt -= 3; else animCnt += 3;
      usr_flag1 = usr_flag0;
    }
    tick++;
    if((tick & 3) == 3) {
      animCnt++;
      if(usr_flag0) {
        if(animCnt >= animations) animCnt = 3;
      } else {
        if(animCnt > 2) animCnt = 0;
      }
    }
  }
};

class Bullet : public AbstractSprite {
public:
  Bullet() : AbstractSprite(BULLET, 1) {
    addSprite(SingleSprite(Dimension(14, 14), (short unsigned int*)bullet));
    keepInMemory = true;
  }

  void onTick() {
    tick++;
    usr_dx = usr_dx + usr_ddx;
    usr_dy = usr_dy + usr_ddy;
    pos.x += (usr_dx >> 10) + (usr_dx >> 11);
    pos.y += (usr_dy >> 10) + (usr_dy >> 11);
    if((pos.x < -10) || (pos.y < 0)) {
      status = VANISHED;
      pos.x = 0xffff;
    }
  }
};

class Hunter : public AbstractSprite {
private:
  Bullet* bullet = NULL;  
public:
  Hunter() : AbstractSprite(HUNTER, 9) {
    addSprite(SingleSprite(Dimension(28, 31), (short unsigned int*)man01, Point(-3, 0)));
    addSprite(SingleSprite(Dimension(21, 35), (short unsigned int*)man02, Point(5, -4)));
    addSprite(SingleSprite(Dimension(18, 37), (short unsigned int*)man03, Point(7, -6)));
    addSprite(SingleSprite(Dimension(19, 37), (short unsigned int*)man04, Point(7, -6)));
    addSprite(SingleSprite(Dimension(21, 35), (short unsigned int*)man05, Point(7, -4)));
    addSprite(SingleSprite(Dimension(29, 31), (short unsigned int*)man06, Point(7, 0)));
    addSprite(SingleSprite(Dimension(25, 32), (short unsigned int*)man07, Point(5, 0)));
    addSprite(SingleSprite(Dimension(31, 37), (short unsigned int*)man08, Point(2, -4)));
    addSprite(SingleSprite(Dimension(44, 11), (short unsigned int*)man09, Point(0, 20)));
    pos.y = 119;
    pos.x = 310;
  }

  void onTick() {
    tick++;
    pos.x--;
    if(pos.x < -40) {
      status = VANISHED;
      pos.x = 0xffff;
    }
    if(status == COLLIDED) {
      if((tick & 7) == 7) {
        if(animCnt < (animations-1)) animCnt++;
      }
    } else {
      if (daisyMode == FLYING && status != VANISHED) {
        if(((tick & 7) == 7) && (daisyPos.x != 0xffff)) {
          int dxDaisy = pos.x - daisyPos.x;
          int dyDaisy = 89-(abs(pos.y - daisyPos.y));
          if(dxDaisy < 0) {
            if(dxDaisy < -(180+dyDaisy)) animCnt = 5; else if(dxDaisy > -(70+dyDaisy)) animCnt = 3; else animCnt = 4;
          } else {
            if(dxDaisy > 160-dyDaisy) animCnt = 0; else if (dxDaisy > 70-dyDaisy) animCnt = 1; else animCnt = 2;
          }
        }
  #ifdef HUNTER_SHOOTS
        if((usr_a <= 0) && (daisyPos.x != 0xffff) && (tick & 0x07) == 0x07) {
          if(rnd_diff(usr_c, 0x0f) == 0x0f) {
            Point p;
            if(animCnt == 0) p = Point(pos.x-12, pos.y-16);
              else if (animCnt == 1) p = Point(pos.x-4, pos.y-24);
                else if(animCnt == 4) p = Point(pos.x+8, pos.y-16);
                  else if(animCnt == 5) p = Point(pos.x+16, pos.y-8);
                    else p = Point(pos.x+4, pos.y-16);
            bullet = new Bullet();
            int dx = daisyPos.x - pos.x - rnd(0xf) + rnd(0x7);
            int dy = daisyPos.y - pos.y - rnd(0xf) + rnd(0x7);
            usr_a = 50;
            bullet->setPos(p);
            bullet->setUsrDxDy(0, 0, dx, dy);
            setSubSprite(bullet);
          }
        }
  #endif      
        usr_a--;
      }
    }
  }
};

class Gras : public AbstractSprite {
public:
  Gras() : AbstractSprite(GRAS, 1) {
    addSprite(SingleSprite(Dimension(7, 11), (short unsigned int*)gras_raw));
    pos.x = 310;
    pos.y = 123;
  }

  void onTick() {
    pos.x--;
    if(pos.x < -3) {
      status = VANISHED;
      pos.x = 0xffff;
    }
  }
};

class Wolf : public AbstractSprite {
public:
  Wolf() : AbstractSprite(WOLF, 5) {
    addSprite(SingleSprite(Dimension(59, 11), (short unsigned int*)wulf01));
    addSprite(SingleSprite(Dimension(61, 11), (short unsigned int*)wulf02, Point(2,0)));
    addSprite(SingleSprite(Dimension(32, 32), (short unsigned int*)crashwolf01, Point(7,-10)));
    addSprite(SingleSprite(Dimension(32, 32), (short unsigned int*)crashwolf02, Point(7,-10)));
    addSprite(SingleSprite(Dimension(32, 32), (short unsigned int*)crashwolf03, Point(7,-10)));
    usr_flag0 = false;
    usr_flag1 = false;
    pos.x = 310;
    if(daisyPos.x != 0xffff) pos.y = daisyPos.y + (rnd() >> 4); else pos.y = 40+rnd(0x1f);
  }

  void onTick() {
    tick++;
    if((tick & 3) == 3) {
      if(((daisyPos.x-8) > pos.x) && (rnd(3) == 3)) pos.x--;
      if(((daisyPos.x+8) < pos.x) && (rnd(3) == 3)) pos.x++;
      animCnt++;
      if(status == COLLIDED) {
        if(animCnt < 2) animCnt = 2;
        if(animCnt >= animations) animCnt = 2;  
      } else {
        if(animCnt > 1) animCnt = 0;
      }
    }
    pos.x-=2;
    if((tick & 1) == 1) pos.x--;
    if(pos.x < -58) {
      status = VANISHED;
      pos.x = 0xffff;
    }
  }
};

class Life : public AbstractSprite {
public:
  Life() : AbstractSprite(LIFE, 1) {
    addSprite(SingleSprite(Dimension(7, 11), (short unsigned int*)life_raw));
    keepInMemory = true;
  }

  void onTick() {}
};

class DaisyInPeaces : public AbstractSprite {
private:
  int x[7];
  int y[7];
  int dx[7];
  int dy[7];
  int ddy[7];
  bool ready;  
public:
  DaisyInPeaces(int daisyX, int daisyY) : AbstractSprite (DAISY_IN_PEACES, 7) {
    addSprite(SingleSprite(Dimension(6, 11), (short unsigned int*)pieces01));
    addSprite(SingleSprite(Dimension(9, 10), (short unsigned int*)pieces02));
    addSprite(SingleSprite(Dimension(10, 11), (short unsigned int*)pieces03));
    addSprite(SingleSprite(Dimension(9, 9), (short unsigned int*)pieces04));
    addSprite(SingleSprite(Dimension(9, 10), (short unsigned int*)pieces05));
    addSprite(SingleSprite(Dimension(6, 4), (short unsigned int*)pieces06));
    addSprite(SingleSprite(Dimension(5, 4), (short unsigned int*)pieces07));
    for(int n=0; n<animations; n++) {
      x[n] = daisyX;
      y[n] = daisyY;
      dx[n] = 4-rnd(7);
      dy[n] = -2 - rnd(3);
      ddy[n] = rnd(1) + 1;
    }
    pos.x = daisyPos.x;
    pos.y = daisyPos.y;
    ready = false;
  }

  void onTick() {
    tick++;
    ready = false;
    if((tick & 0x1) == 0x1) {
      for(int n=0; n<animations; n++) {
        x[n] = x[n] + dx[n];
        y[n] = y[n] + dy[n];
        if((tick & 3) == 3) dy[n] += ddy[n];
        if(y[n] > 200) ready = true;
      }
    }
  }

  bool isReady() {
    return ready;
  }

  void drawAllOnBackground(LGFX_Sprite* background) {
    for(int n=0; n<animations; n++) {
       if(lgfxSprite.createSprite(sprites[n].dimension.width, sprites[n].dimension.height)) {
        lgfxSprite.setSwapBytes(true);
        lgfxSprite.pushImage(0, 0, sprites[n].dimension.width, sprites[n].dimension.height, &sprites[n].ptr[0]);
        // Serial.printf("push sprite to %d, %d\n", x[n], y[n]);
        lgfxSprite.pushSprite(background, x[n], y[n], 0x0000);
        lgfxSprite.deleteSprite();
      } else {
        Serial.println("Out of memory!");
        sleep(10000);
      }
    }
  }
};

class Numbers : public AbstractSprite {
public:  
  Numbers() : AbstractSprite(NUMBERS, 10) {
    addSprite(SingleSprite(Dimension(7, 10), (short unsigned int*)number0));
    addSprite(SingleSprite(Dimension(7, 10), (short unsigned int*)number1));
    addSprite(SingleSprite(Dimension(7, 10), (short unsigned int*)number2));
    addSprite(SingleSprite(Dimension(7, 10), (short unsigned int*)number3));
    addSprite(SingleSprite(Dimension(7, 10), (short unsigned int*)number4));
    addSprite(SingleSprite(Dimension(7, 10), (short unsigned int*)number5));
    addSprite(SingleSprite(Dimension(7, 10), (short unsigned int*)number6));
    addSprite(SingleSprite(Dimension(7, 10), (short unsigned int*)number7));
    addSprite(SingleSprite(Dimension(7, 10), (short unsigned int*)number8));
    addSprite(SingleSprite(Dimension(7, 10), (short unsigned int*)number9));
  }

  void onTick() {
  }

};

#endif
