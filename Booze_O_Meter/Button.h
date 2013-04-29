// Copyright (c) 2013 Mark Dyer. All rights reserved.
#ifndef BUTTON_H
#define BUTTON_H

#include "../base/base.h"

namespace mdlib {

  // Button wiring:
  //
  //               /  
  //  pin  -------/  ------- 5V
  //          |
  //          ---/\/\/\/---- GND
  //              10K R
  //
  // update() is what sets the state of the button. update should be called
  // before the state of the button is queried.
  // 
  class Button : public DigitalInput {
  public:
    enum State { UP, DOWN };

    Button();

    virtual void update();
    bool isPressed() const { return state_ == DOWN; }

  private:
    enum ClickState {
      IDLE,
      IN_CLICK,
      LONG_CLICK,
      MAYBE_DOUBLE,
      DOUBLE_CLICK
    };
    
    State state_;
    State previous_state_;
    ClickState click_state_;
    State debounce();

    unsigned long last_up_;
    unsigned long last_down_;
  };
}
#endif // BUTTON_H
