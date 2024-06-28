#ifndef _TITLE_HPP_
#define _TITLE_HPP_

#include "AbstractSprite.h"
#include "gfx.h"

class Title : public AbstractSprite {
private:
  int dt = 1;
  int ddt = 1;

public:
  Title() : AbstractSprite(-1, 14) {
    addSprite(SingleSprite(Dimension(34, 29), (short unsigned int*)title_A));
    addSprite(SingleSprite(Dimension(27, 28), (short unsigned int*)title_S));
    addSprite(SingleSprite(Dimension(24, 26), (short unsigned int*)title_T));
    addSprite(SingleSprite(Dimension(33, 29), (short unsigned int*)title_R));
    addSprite(SingleSprite(Dimension(33, 26), (short unsigned int*)title_O));
    addSprite(SingleSprite(Dimension(25, 21), (short unsigned int*)logo_Mc));
    addSprite(SingleSprite(Dimension(14, 16), (short unsigned int*)logo_c, Point(0, 12)));
    addSprite(SingleSprite(Dimension(28, 26), (short unsigned int*)title_C));
    addSprite(SingleSprite(Dimension(34, 31), (short unsigned int*)title_H));
    addSprite(SingleSprite(Dimension(18, 26), (short unsigned int*)title_I));
    addSprite(SingleSprite(Dimension(30, 28), (short unsigned int*)title_C_1));
    addSprite(SingleSprite(Dimension(34, 31), (short unsigned int*)title_K));
    addSprite(SingleSprite(Dimension(27, 29), (short unsigned int*)title_E));
    addSprite(SingleSprite(Dimension(34, 31), (short unsigned int*)title_N));
    tick = 30;
  }

  void setDaisyPos(Point _p) { }
  void setEggPos(Point _p) { }

  void onTick() {
    tick--;
    if(tick < 1) tick = 60;
    pos.x-=2;
    if(pos.x < -160) {
      pos.x = 210;
      dt = dt + ddt;
      if(dt > 4) {
        ddt = -1;
      } else if (dt < 2) {
        ddt = 1;
      }
    }
  }

  void drawAllOnSprite(LGFX_Sprite* background) {
    int titleX = pos.x;
    int t = tick;
    for(int n=0; n<animations; n++) {
      float radian = TWO_PI / (60 / (float)(t));
      float titleY = pos.y + (29 * sin(radian));
      t+=dt;
      if(t >= 60) t-=60;
      if(lgfxSprite.createSprite(sprites[n].dimension.width, sprites[n].dimension.height)) {
        lgfxSprite.setSwapBytes(true);
        lgfxSprite.pushImage(0, 0, sprites[n].dimension.width, sprites[n].dimension.height, &sprites[n].ptr[0]);
        lgfxSprite.pushSprite(background, pos.x + titleX, titleY, 0x0000);
        titleX += (sprites[n].dimension.width - 4);
        lgfxSprite.deleteSprite();
      } else {
        Serial.println("Out of memory!");
        sleep(10000);
      }
    }
  }
};

#endif