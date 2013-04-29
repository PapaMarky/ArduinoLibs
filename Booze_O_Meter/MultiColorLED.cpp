#include "MultiColorLED.h"

namespace mdlib {
  MultiColorLED::MultiColorLED() 
  {
  }

  void MultiColorLED::setup() const {
    red_.setup();
    green_.setup();
    blue_.setup();
  }

  void MultiColorLED::set_pins(int red_pin, int green_pin, int blue_pin) {
    red_.set_pin(red_pin);
    green_.set_pin(green_pin);
    blue_.set_pin(blue_pin);
  }

  void MultiColorLED::set_color(uint32_t color) const {
    red_.setLevel(  (byte)((color & RED) >> 16));
    green_.setLevel((byte)((color & GREEN) >> 8));
    blue_.setLevel( (byte) (color & BLUE));
  }

  void MultiColorLED::blend_colors(uint32_t c1, uint32_t c2, float pct) const {
    if (pct <= 0.0)
      set_color(c1);
    else if (pct >= 1.0)
      set_color(c2);
    else {
      int r1 = (c1 & RED) >> 16;
      int g1 = (c1 & GREEN) >> 8;
      int b1 = c1 & BLUE;
      int r2 = (c2 & RED) >> 16;
      int g2 = (c2 & GREEN) >> 8;
      int b2 = c2 & BLUE;

      int r = r1 + (int)(pct * (float)(r2 - r1));
      int g = g1 + (int)(pct * (float)(g2 - g1));
      int b = b1 + (int)(pct * (float)(b2 - b1));

      set_color(
		((r & 0xff) << 16) |
		((g & 0xff) << 8) |
		(b & 0xff));
    }
  }
} // namespace mdlib
