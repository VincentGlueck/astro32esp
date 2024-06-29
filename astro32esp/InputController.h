#ifndef _INPUT_CONTROLLER_
#define _INPUT_CONTROLLER_

#include "TheTouch.h"
#include "SimpleScreenTexts.h"
#include "AbstractSprite.h"

#define FASTEST_REPEAT_MS 120
#define KEEP_TOUCH_FOR_MS 80

enum UserInput { T_NOTHING = 0,
                 T_UP = 1,
                 T_LEFT = 2,
                 T_RIGHT = 4,
                 T_DOWN = 8,
                 T_FIRE = 16
};

class InputController {
public:
  class ControllerCallback : public TheTouch::TouchCallback {
    public:
      virtual void clicked(lgfx::touch_point_t* tp);
      virtual uint8_t getInput();
      virtual void processed();
      virtual void setCalibrated(bool _cal);
      virtual void setPPos(Point _point);
      virtual Point getLatestTouch();
    };

public:
  InputController(LGFX* _lcd);
  ~InputController();
  uint8_t getInput();
  void calibrate();
  void setCalibrated(bool _cal);
  void processed();
  void poll();
  void setPlayerPos(Point _point);
  bool isTouched();
  Point getLatestTouch();

private:
  TheTouch* touch;
  ControllerCallback* callback;
  lgfx::touch_point_t* tp;
  SimpleScreenTexts* screenTexts;
  LGFX* lcd;

};

#endif