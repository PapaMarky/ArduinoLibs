// Copyright (c) 2013 Mark Dyer. All rights reserved.

#include "Button.h"
#include "EventQueue.h"

namespace mdlib {
    
  Button::Button() : state_(UP), previous_state_(UP), click_state_(IDLE)
  {
    last_up_ = last_down_ = millis();
  }

  const unsigned long LONG_CLICK_MIN_DURATION = 1000;
  const unsigned long DOUBLE_CLICK_MAX_WAIT = 100;
  
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

    state_ = state ? UP : DOWN;

    if (state_ != previous_state_) {
      PostEvent(Event(state_ == HIGH ? Event::BUTTON_UP : Event::BUTTON_DOWN, (int)this));
      previous_state_ = state_;

      if (state_ == UP) {
	last_up_ = millis();

	if (click_state_ == IN_CLICK) {
	  if (millis() - last_down_ >= LONG_CLICK_MIN_DURATION) {
	    PostEvent(Event(Event::BUTTON_LONG_CLICK, (int)this));
	    click_state_ = IDLE;
	  }
	  else
	    click_state_ = MAYBE_DOUBLE;
	}
      }
      else {
	last_down_ = millis();

	if (click_state_ == IDLE)
	  click_state_ = IN_CLICK;
	else if (click_state_ == MAYBE_DOUBLE) {
	  PostEvent(Event(Event::BUTTON_DOUBLE_CLICK, (int)this));
	  click_state_ = IDLE;
	}
      }
    }
    else if (click_state_ == MAYBE_DOUBLE && millis() - last_up_ >= DOUBLE_CLICK_MAX_WAIT)
      click_state_ = IDLE;

    return (state_ == HIGH ? UP : DOWN);
  }
}
