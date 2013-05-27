// Copyright (c) 2013 Mark Dyer. All rights reserved.
#ifndef LIGHTED_BUTTON_H
#define LIGHTED_BUTTON_H

#include "../base/base.h"
#include "Button.h"

namespace mdlib {

  class LightedButton {
   public:
    LightedButton() : brightness_(1.0f) {}
    ~LightedButton() {}
    
    void set_pins(int button_pin, int led_pin);

    void setup();
    void update();

    void SetBrightness(float brightness) { brightness_ = brightness; }
    void TurnOn() const { led_.setLevel(brightness_); }
    void TurnOff() const { led_.setLevel(0.0f); }

    bool IsPressed() { return button_.isPressed(); }

  private:
    float brightness_;
    Button button_;
    AnalogOutput led_;
  };
} // namespace mdlib

#endif // LIGHTED_BUTTON_H
