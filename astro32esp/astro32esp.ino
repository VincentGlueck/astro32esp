#include "lgfx_user.h"
#include "TheTouch.h"

#include "background.h"
#include "daisy.h"

LGFX lcd;

#define ORIENTATION 3 // 3=POWER left 1=POWER right
#define USE_SERIAL_OUT

const long TOUCH_REPEAT = 80;            // how often (ms) is (long)touch possible
const int SCREEN_LEFT = 7;
const int SCREEN_TOP = 32;
const int SCREEN_WIDTH = 306;
const int SCREEN_HEIGHT = 160;

uint32_t globalCnt = 0;
long lastMillis;
long lastTouch = 0;
bool isCalibrated = false;

uint8_t mode = 0;
long nextMode = millis() + 30000;
uint8_t modeDone = 0xff;

enum UserInput { Nothing = 0,
                 Up = 1,
                 Left = 2,
                 Right = 3,
                 Down = 4
};
enum UserInput input = Nothing;

uint8_t za, zb, zc, zx, counter;
uint8_t rnd() {
  zx++;
  za = (za ^ zc ^ zx);
  zb = (zb + za);
  zc = ((zc + (zb >> 1)) ^ za);
  return zc;
}

uint16_t score;

void showInputAsString() {
  switch (input) {
    case 0: break;
    case 1: Serial.println("Rotate"); break;
    case 2: Serial.println("Left"); break;
    case 3: Serial.println("Right"); break;
    case 4: Serial.println("Down"); break;
    default: break;
  }
}

class TCallback : public TheTouch::TouchCallback {
public:
  void clicked(lgfx::touch_point_t *tp) {
    if (millis() > lastTouch) {
#ifdef _SHOW_DEBUG_
      Serial.printf("touch y: %d, x: %d\n", tp->y, tp->x);
#endif
      if (isCalibrated) {
        if (abs(tp->y) > lcd.height() - 40) {
          input = Down;
        } else {
          if (abs(tp->x) < 100) {
            input = Left;
          } else if (abs(tp->x) > (lcd.width() - 80)) {
            input = Right;
          } else {
            input = Up;
          }
        }
      } else {
        if (abs(tp->y) < 50) {
          input = Down;
        } else {
          if (abs(tp->x) < 70) {
            input = Right;
          } else if (abs(tp->x) > (lcd.width() - 70)) {
            input = Left;
          } else {
            input = Up;
          }
        }
      }
#ifdef _SHOW_DEBUG_
      showInputAsString();
#endif
      lastTouch = millis() + TOUCH_REPEAT;
    }
  }
};

lgfx::touch_point_t *tp;
TCallback tCallback;
TheTouch touch(lcd, &tCallback);

// TODO
static LGFX_Sprite sprites[3];
static LGFX_Sprite daisy;

void bigScreen(String str, int color, bool clear = true) {
  if (clear) restoreBg();
  lcd.setFont(&fonts::DejaVu24);
  lcd.setTextDatum(textdatum_t::middle_center);
  lcd.setTextColor(color);
  lcd.drawString(str, lcd.width() >> 1, lcd.height() >> 1);
  lcd.drawString(str, 1 + (lcd.width() >> 1), 1 + (lcd.height() >> 1));
}

void smallScreen(String str, int color = TFT_BLACK) {
  lcd.setFont(&fonts::DejaVu12);
  lcd.setTextColor(color);
  lcd.drawString(str, lcd.width() >> 1, (lcd.height() >> 1) + 32);
  lcd.drawString(str, (lcd.width() >> 1) + 1, (lcd.height() >> 1) + 33);
}

void initSprites() {
  if(!daisy.createSprite(121, 108)) {
    Serial.println("Unable to create Daisy.");
    return;
  }
  daisy.setSwapBytes(true);
  daisy.pushImage(0, 0, 121, 108, &daisy_raw[0]);
}

void restoreBg() {
  long t = millis();
  lcd.pushImage(0, 0, lcd.width(), 38, &back_top[0]);
  lcd.pushImage(0, lcd.height()-55, lcd.width(), 54, &back_bottom[0]);
  for(int n=38; n<185; n+=2) {
    lcd.pushImage(0,n, 5, 2, &back_left[0]);
    lcd.pushImage(lcd.width()-5, n, 5, 2, &back_right[0]);
  }
  t = millis() - t;
  Serial.printf("restoreBg() took %d ms\n", t);
}

void drawDaisy(int x, int y) {
  daisy.pushSprite(&lcd, x, y, 0x0000);
}


void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);
  tp = (lgfx::touch_point_t *)malloc(sizeof(lgfx::touch_point_t) << 1);
  lcd.init();
  lcd.setRotation(ORIENTATION);
  lcd.setSwapBytes(true);
  initSprites();
  lastMillis = millis();
  za = random(256);
  zb = random(256);
  zc = random(256);
  zx = random(256);
}

void loop() {
  restoreBg();
  sleep(10); // DO NOTHING
  drawDaisy((lcd.width() >> 1) - 82, 56);
  drawDaisy((lcd.width() >> 1) - 72, 64);
}