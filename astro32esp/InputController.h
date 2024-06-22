#ifndef _INPUT_CONTROLLER_
#define _INPUT_CONTROLLER_

#include "TheTouch.h"
#include "SimpleScreenTexts.h"

#define FASTEST_REPEAT_MS 120
#define KEEP_TOUCH_FOR_MS 80

enum UserInput { Nothing = 0,
                 Up = 1,
                 Left = 4,
                 Right = 8,
                 Down = 2,
                 Fire = 16
};

class InputController {
public:
  class ControllerCallback : public TheTouch::TouchCallback {
    public:
      virtual void clicked(lgfx::touch_point_t* tp);
      virtual uint8_t getInput();
      virtual void processed();
      virtual void setCalibrated(bool _cal);
  };

public:
  InputController(LGFX _lcd);
  ~InputController();
  uint8_t getInput();
  void debugAsAscii();
  void calibrate();
  void setCalibrated(bool _cal);
  void processed();
  void poll();

private:
  TheTouch* touch;
  ControllerCallback* callback;
  lgfx::touch_point_t* tp;
  SimpleScreenTexts* screenTexts;
  LGFX lcd;

};

#endif