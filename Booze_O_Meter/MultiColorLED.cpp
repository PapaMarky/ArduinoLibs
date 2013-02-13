#include "MultiColorLED.h"

namespace mdlib {
  MultiColorLED::MultiColorLED() 
  {
  }

  void MultiColorLED::setup() {
    red_.setup();
    green_.setup();
    blue_.setup();
  }

  void MultiColorLED::set_pins(int red_pin, int green_pin, int blue_pin) {
    red_.set_pin(red_pin);
    green_.set_pin(green_pin);
    blue_.set_pin(blue_pin);
  }

  void MultiColorLED::set_color(uint32_t color) {
    red_.setLevel(  (byte)((color & RED) >> 16));
    green_.setLevel((byte)((color & GREEN) >> 8));
    blue_.setLevel( (byte) (color & BLUE));
  }
} // namespace mdlib
