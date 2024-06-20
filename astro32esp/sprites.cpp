#ifndef _SPRITES_CPP_
#define _SPRITES_CPP_

#include "AbstractSprite.h"
#include "daisy.h"

class Daisy : public AbstractSprite {
public:
  Daisy() : AbstractSprite("Daisy", 1 ) {
    addSprite(SingleSprite(Dimension(121, 108), (short unsigned int*)daisy_raw));
    keepInMemory = true;
  }

  void onTick() {
    pos.x++;
    if(pos.x > 310) pos.x = -120;
  }

};

#endif
