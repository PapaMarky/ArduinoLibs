// Copyright (c) 2013 Mark Dyer. All rights reserved.

#include "Button.h"

namespace mdlib {
  Button::Button() : state_(UP), previous_state_(UP) {}

  void Button::update() {
    state_ = debounce(); 
  }

  Button::State Button::debounce() {
    const int debounceDelay = 10;
    bool state;
    bool previousState;
    previousState = getState();
    
    for (int counter = 0; counter < debounceDelay; counter++) {
      delay(1);
      state = getState();
      if (state != previousState) {
	counter = 0;
	previousState = state;
      }
    }

    if (state_ != previous_state_) {
	
    }
    
    return (state == HIGH ? UP : DOWN);
  }
}
