#ifndef STYLED_NUMBER_HPP_
#define STYLED_NUMBER_HPP_

#include "sprites.hpp"

class StyledNumber {

private:
  LGFX* lcd;
  Numbers* digits;
  LGFX_Sprite numberSprite;
  uint8_t maxDigits;
  uint16_t numberBgColor;

public:
  StyledNumber(LGFX* _lcd, uint8_t _maxDigits, uint16_t _bgColor = TFT_BLACK) {
    lcd = _lcd;
    maxDigits = _maxDigits;
    numberSprite.createSprite(maxDigits*8, 12);
    digits = new Numbers();
    numberBgColor = _bgColor;
  }

  ~StyledNumber() {
    if(digits != NULL) delete digits;
  }

  void drawNumber(int number, int posX, int posY) {
    numberSprite.fillRect(0, 0, numberSprite.width(), numberSprite.height(), numberBgColor);
    int x = (maxDigits-1)*8;
    if(number == 0) {
      digits->setAnimCnt(0);
      numberSprite.pushSprite(lcd, posX, posY);
    }
    for (int digit = number; digit != 0; digit /= 10) {
      int last_digit = digit % 10;
      digits->setAnimCnt(last_digit);
      digits->setPos(Point(x, rnd(1)));
      digits->drawOnSprite(&numberSprite);
      x-=8;
    }
    numberSprite.pushSprite(lcd, posX, posY);
  }
};

#endif