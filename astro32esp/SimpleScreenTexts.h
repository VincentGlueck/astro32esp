#ifndef _SIMPLE_SCREEN_TEXTS_
#define _SIMPLE_SCREEN_TEXTS_

#include "lgfx_user.h"
#include "SimpleScreenTexts.h"

class SimpleScreenTexts {
public:
  SimpleScreenTexts(LGFX _lcd, uint16_t _bgColor = TFT_BLACK);
  ~SimpleScreenTexts();
  void bigText(String str, int color, bool clear = true);
  void smallText(String str, int color = TFT_WHITE, int deltaY = 0);
  void spriteBig(String str, LGFX_Sprite sprite, int color);
  void spriteSmall(String str, LGFX_Sprite sprite, int color = TFT_WHITE, int deltaY = 0);
private:
  uint16_t bgColor;
  LGFX lcd;
};

#endif