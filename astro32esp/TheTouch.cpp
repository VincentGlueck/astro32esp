#include "TheTouch.h"

TheTouch::TheTouch(LGFX _lgfx) {
  lcd = _lgfx;
}

TheTouch::TheTouch(LGFX _lgfx, TouchCallback* _callback) {
  lcd = _lgfx;
  callback = _callback;
  tp = (lgfx::touch_point_t*)malloc(sizeof(lgfx::touch_point_t));
}

TheTouch::~TheTouch() {
  free(tp);
}

bool TheTouch::get(lgfx::touch_point_t *tp) {
  return lcd.getTouch(tp);
}

bool TheTouch::getRaw(lgfx::touch_point_t *tp) {
  return lcd.getTouchRaw(tp);
}

void TheTouch::poll() {
  lgfx::touch_point_t *_tp = (lgfx::touch_point_t*)malloc(sizeof(lgfx::touch_point_t));
  if(lcd.getTouch(_tp)) {
    callback->clicked(_tp);
  }
  free(_tp);
}