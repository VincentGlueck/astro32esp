#ifndef _STUCTS_H_
#define _STUCTS_H_

struct SpriteStruct {
  char character;
  int width;
  int height;
  int dx;
  int dy;
  const unsigned short * raw;
};

#endif