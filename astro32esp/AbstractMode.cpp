#include "AbstractMode.h"

AbstractMode::AbstractMode(LGFX* _lcd, LGFX_Sprite* _background, InputController* _inputController, Scroller* _scroller) {
  lcd = _lcd;
  background = _background;
  inputController = _inputController;
  scroller = _scroller;
  initialized = false;
}

uint8_t AbstractMode::getNextMode() {
  return currentMode;
}

void AbstractMode::restoreBg() {
  lcd->pushImage(0, 0, lcd->width(), 38, &back_top[0]);
  lcd->pushImage(0, lcd->height()-55, lcd->width(), 54, &back_bottom[0]);
  for(int n=38; n<185; n+=2) {
    lcd->pushImage(0,n, 5, 2, &back_left[0]);
    lcd->pushImage(lcd->width()-5, n, 5, 2, &back_right[0]);
  }
}

void AbstractMode::clearPlayfield() {
  background->setSwapBytes(true);
  background->fillRect(0, 0, background->width(), background->height()-EXTRA_BOTTOM, TFT_BLACK);
  background->pushImage(-SCREEN_LEFT, background->height()-EXTRA_BOTTOM, lcd->width(), EXTRA_BOTTOM, &back_bottom[0]);
}

void AbstractMode::drawPlayfield() {
  background->pushSprite(lcd, SCREEN_LEFT, SCREEN_TOP);
}