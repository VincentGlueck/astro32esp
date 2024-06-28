#ifndef _ABSTRACT_MODE_H
#define _ABSTRACT_MODE_H

#include "lgfx_user.h"
#include "background.h"
#include "AbstractSprite.h"
#include "InputController.h"
#include "Scroller.h"

enum GameModes {
  HELLO,
  MENU,
  IN_GAME
};

#define EXTRA_BOTTOM 28
#define SCREEN_LEFT 5
#define SCREEN_TOP 37
const int SCREEN_WIDTH = PLAYFIELD_WIDTH;
const int SCREEN_HEIGHT = PLAYFIELD_HEIGHT;

class AbstractMode {
public:
  AbstractMode(LGFX* _lcd, LGFX_Sprite* _background, InputController* _inputController = NULL, Scroller* _scroller = NULL);
  uint8_t getNextMode();
  void restoreBg();
  void clearPlayfield();
  void drawPlayfield();
  virtual void onTick() {};
  virtual void killMode() {};

protected:
  bool initialized;
  uint8_t currentMode;
  LGFX* lcd;
  LGFX_Sprite* background;
  InputController* inputController;
  Scroller* scroller;

};

#endif