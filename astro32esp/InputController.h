#ifndef _INPUT_CONTROLLER_
#define _INPUT_CONTROLLER_

#include "TheTouch.h"
#include "SimpleScreenTexts.h"

#define FASTEST_REPEAT_MS 20
#define KEEP_TOUCH_FOR_MS 40

enum UserInput { Nothing = 0,
                 Up = 1,
                 Left = 2,
                 Right = 3,
                 Down = 4,
                 Fire = 5
};

class InputController {
public:
  class ControllerCallback : public TheTouch::TouchCallback {
    public:
      virtual void clicked(lgfx::touch_point_t* tp);
      virtual UserInput getInput();
  };

public:
  InputController(LGFX _lcd);
  ~InputController();
  UserInput getInput();
  void debugAsAscii();
  void calibrate();
  void poll();

private:
  TheTouch* touch;
  ControllerCallback* callback;
  lgfx::touch_point_t* tp;
  SimpleScreenTexts* screenTexts;
  LGFX lcd;
  bool isCalibrated;

};

#endif