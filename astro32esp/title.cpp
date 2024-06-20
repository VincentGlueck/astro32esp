#ifndef _TITLE_H_
#define _TITLE_H_

#include "AbstractSprite.h"
#include "gfx.h"

/*
{'A', 34, 29, 0, 0, title_A},
{'S', 27, 28, 0, 0, title_S},
{'T', 24, 26, 0, 0, title_T},
{'R', 33, 29, 0, 0, title_R},
{'O', 33, 26, 0, 0, title_O},
{' ', 25, 21, 0, 0, logo_Mc},
{'C', 28, 26, 0, 0, title_C},
{'H', 34, 31, 0, 0, title_H},
{'I', 18, 26, 0, 0, title_I},
{'C', 30, 28, 0, 0, title_C_1},
{'K', 34, 31, 0, 0, title_K},
{'E', 27, 29, 0, 0, title_E},
{'N', 34, 31, 0, 0, title_N},
*/


class Title : public AbstractSprite {
public:
  Title() : AbstractSprite("title", 13) {
    addSprite(SingleSprite(Dimension(34, 29), (short unsigned int*)title_A));
    addSprite(SingleSprite(Dimension(27, 28), (short unsigned int*)title_S));
    addSprite(SingleSprite(Dimension(24, 26), (short unsigned int*)title_T));
    addSprite(SingleSprite(Dimension(33, 29), (short unsigned int*)title_R));
    addSprite(SingleSprite(Dimension(33, 26), (short unsigned int*)title_O));
    addSprite(SingleSprite(Dimension(25, 21), (short unsigned int*)logo_Mc));
    addSprite(SingleSprite(Dimension(28, 26), (short unsigned int*)title_C));
    addSprite(SingleSprite(Dimension(34, 31), (short unsigned int*)title_H));
    addSprite(SingleSprite(Dimension(18, 26), (short unsigned int*)title_I));
    addSprite(SingleSprite(Dimension(30, 28), (short unsigned int*)title_C_1));
    addSprite(SingleSprite(Dimension(34, 31), (short unsigned int*)title_K));
    addSprite(SingleSprite(Dimension(27, 29), (short unsigned int*)title_E));
    addSprite(SingleSprite(Dimension(34, 31), (short unsigned int*)title_N));
    tick = 30;
  }

  void onTick() {
    tick--;
    if(tick < 1) tick = 60;
    pos.x-=2;
    if(pos.x < -220) pos.x = 220;
  }

  void drawAllOnSprite(LGFX_Sprite* background) {
    int titleX = pos.x;
    int t = tick;
    for(int n=0; n<animations; n++) {
      float radian = PI / (60 / (float)(t));
      float titleY = pos.y + (40 * sin(radian));
      t+=4;
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