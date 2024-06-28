#ifndef _MENU_MODE_HPP_
#define _MENU_MODE_HPP_

#include "AbstractMode.h"
#include "title.hpp"
#include "sprites.hpp"

class MenuMode : public AbstractMode {
private:
  Title* title;
  GetReady* getReady;
  long menuPollDelay;

public:
  MenuMode(LGFX* _lcd, LGFX_Sprite* _background, InputController* _inputController, Scroller* _scroller)
    : AbstractMode(_lcd, _background, _inputController, _scroller) {
    currentMode = MENU;
    title = new Title();
    getReady = new GetReady();
    menuPollDelay = millis() + 100;
  }

  void killMode() {
    if(title != NULL) delete title;
    if(getReady != NULL) delete getReady;
  }

  void onTick() {
    if (!initialized) {
      restoreBg();
      title->setPos(Point(320, 35));
      initialized = true;
    }
    clearPlayfield();
    scroller->onTick();
    title->drawAllOnSprite(background);
    if (getReady->getStatus() == NORMAL) getReady->drawOnSprite(background);
    getReady->onTick();
    title->onTick();
    drawPlayfield();
    if (millis() > menuPollDelay) {
      inputController->poll();
      if (inputController->isTouched()) {
        currentMode = IN_GAME;
      }
      menuPollDelay = millis() + 100;
    }
  }
};

#endif