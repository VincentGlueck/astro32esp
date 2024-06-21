#ifndef _FAST_RANDOM_HPP_
#define _FAST_RANDOM_HPP_

#include "Arduino.h"

static uint8_t za = random(256);
static uint8_t zb = random(256);
static uint8_t zc = random(256);
static uint8_t zx = random(256);
static uint8_t rnd(uint8_t mask = 0xff) {
  zx++;
  za = (za ^ zc ^ zx);
  zb = (zb + za);
  zc = ((zc + (zb >> 1)) ^ za);
  return zc & mask;
}

#endif
