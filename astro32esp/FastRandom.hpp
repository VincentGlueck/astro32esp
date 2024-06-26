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

static uint8_t rnd_diff(uint8_t mask = 0xff, uint8_t difficulty = 0) {
  return rnd(((mask << 3) | 0x7) >> (3 - (difficulty & 3)));
}

#endif
