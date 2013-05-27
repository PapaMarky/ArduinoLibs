// Copyright (c) 2013 Mark Dyer. All rights reserved.
#include "LightedButton.h"

namespace mdlib {
  void LightedButton::set_pins(int button_pin, int led_pin) {
    button_.set_pin(button_pin);
    led_.set_pin(led_pin);
  }

  void LightedButton::setup() {
    button_.setup();
    led_.setup();
  }
  void LightedButton::update() {
    button_.update();
    led_.update();
  }
} // namespace mdlib
