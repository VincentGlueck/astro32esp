#ifndef _INPUT_CONTROLLER_
#define _INPUT_CONTROLLER_

#include "TheTouch.h"
#include "SimpleScreenTexts.h"
#include "AbstractSprite.h"

#define FASTEST_REPEAT_MS 120
#define KEEP_TOUCH_FOR_MS 80

enum UserInput { Nothing = 0,
                 Up = 1,
                 Left = 2,
                 Right = 4,
                 Down = 8,
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
      virtual void setDPos(Point _point);
  };

public:
  InputController(LGFX _lcd);
  ~InputController();
  uint8_t getInput();
  void calibrate();
  void setCalibrated(bool _cal);
  void processed();
  void poll();
  void setDaisyPos(Point _point);
  bool isTouched();

private:
  TheTouch* touch;
  ControllerCallback* callback;
  lgfx::touch_point_t* tp;
  SimpleScreenTexts* screenTexts;
  LGFX lcd;

};

#endif