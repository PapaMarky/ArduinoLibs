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
    bool isPressed() { return state_ == DOWN; }

  private:
    State state_;
    State previous_state_;
    State debounce();
  };
}
#endif // BUTTON_H
