// Copyright (c) 2013 Mark Dyer. All rights reserved.
#ifndef LIGHTED_BUTTON_H
#define LIGHTED_BUTTON_H

#include "../base/base.h"
#include "Button.h"

namespace mdlib {

  class LightedButton {
   public:
    LightedButton() : brightness_(1.0f), is_on_(false) {}
    ~LightedButton() {}
    
    void set_pins(int button_pin, int led_pin);

    void setup();
    void update();

    void SetBrightness(float brightness) {
      brightness_ = brightness;
      if (is_on_)
	led_.setLevel(brightness_);
    }
    void TurnOn() { led_.setLevel(brightness_); is_on_ = true; }
    void TurnOff() { led_.setLevel(0.0f); is_on_ = false; }

    bool IsPressed() { return button_.isPressed(); }

  private:
    float brightness_;
    bool is_on_;
    Button button_;
    AnalogOutput led_;
  };
} // namespace mdlib

#endif // LIGHTED_BUTTON_H
