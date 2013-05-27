// Copyright (c) 2013 Mark Dyer. All rights reserved.

#include "Arduino.h"

#include "StateMachine.h"
#include "StateContext.h"
#include "../base/base.h"
#include "BoozeSensor.h"
#include "LightedButton.h"

namespace BOM {

StateContext* State::s_context = 0;

void State::enter_state() {
  start_time_ = millis();
}

////////////////////////////////////////////////////////////////////
//      StartUpState
////////////////////////////////////////////////////////////////////

void StartUpState::enter_state() {
  State::enter_state();

  s_context->fan()->turnOn();
  s_context->sensor()->turnOn();
  s_context->button()->turnOff();
}

State* StartUpState::loop() {
  State* next_state = 0;
  unsigned long elapsed = millis() - start_time_;

  if (elapsed >= 1000) {
    return next_state;
  }
  
  return (State*)0;
}

State* StartUpState::handle_event(mdlib::Event e) {
  return (State*)0;
}
    


} // namespace BOM
