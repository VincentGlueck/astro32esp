#ifndef _THETHOUCH_H
#define _THETHOUCH_H

#include "Arduino.h"
#include "lgfx_user.h"

typedef struct touch_result_struct {
  uint32_t x;
  uint32_t y;
  long when;
} touch_result;

class TheTouch {
public:

  class TouchCallback {
    public:
      virtual void clicked(lgfx::touch_point_t* tp);
  };

  TheTouch(LGFX _lgfx);
  TheTouch(LGFX _lgfx, TouchCallback* _callback);
  ~TheTouch();
  bool get(lgfx::touch_point_t *tp);
  bool getRaw(lgfx::touch_point_t *tp);
  void poll();

private:
  LGFX lcd;
  lgfx::touch_point_t *tp;
  TouchCallback *callback;

};

#endif