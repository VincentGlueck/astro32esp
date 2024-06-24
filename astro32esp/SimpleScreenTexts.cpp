#include "SimpleScreenTexts.h"

SimpleScreenTexts::SimpleScreenTexts(LGFX _lcd, uint16_t _bgColor) {
  lcd = _lcd;
  bgColor = _bgColor;
} 

SimpleScreenTexts::~SimpleScreenTexts() {

}

void SimpleScreenTexts::bigText(String str, int color, bool clear) {
  if (clear) lcd.clear(bgColor);
  lcd.setFont(&fonts::DejaVu24);
  lcd.setTextDatum(textdatum_t::middle_center);
  lcd.setTextColor(color);
  lcd.drawString(str, lcd.width() >> 1, lcd.height() >> 1);
  lcd.drawString(str, 1 + (lcd.width() >> 1), 1 + (lcd.height() >> 1));
}

void SimpleScreenTexts::smallText(String str, int color, int deltaY) {
  lcd.setFont(&fonts::DejaVu12);
  lcd.setTextDatum(textdatum_t::middle_center);
  lcd.setTextColor(color);
  lcd.drawString(str, lcd.width() >> 1, (lcd.height() >> 1) + 30 + deltaY);
  lcd.drawString(str, (lcd.width() >> 1) + 1, (lcd.height() >> 1) + 31 + deltaY);
}

void SimpleScreenTexts::spriteBig(String str, LGFX_Sprite sprite, int color) {
  sprite.setFont(&fonts::DejaVu24);
  sprite.setTextDatum(textdatum_t::middle_center);
  sprite.setTextColor(color);
  sprite.drawString(str, sprite.width() >> 1, (sprite.height() >> 1) - 16);
}

void SimpleScreenTexts::spriteSmall(String str, LGFX_Sprite sprite, int color, int deltaY) {
  sprite.setFont(&fonts::DejaVu12);
  sprite.setTextDatum(textdatum_t::middle_center);
  sprite.setTextColor(color);
  sprite.drawString(str, sprite.width() >> 1, (sprite.height() >> 1) + 12 + deltaY);
}