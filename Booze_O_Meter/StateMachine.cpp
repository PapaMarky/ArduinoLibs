// Copyright (c) 2013 Mark Dyer. All rights reserved.

#include "Arduino.h"

#include "StateMachine.h"
#include "StateContext.h"
#include "../base/base.h"
#include "BoozeSensor.h"
#include "LightedButton.h"
#include "MultiColorLED.h"
#include "SevenSegmentDisplay.h"

namespace mdlib {
  extern const char* event_name(mdlib::Event e);
}

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

  s_context->led()->set_color(mdlib::WHITE);
  s_context->fan()->turnOn();
  s_context->sensor()->turnOn();
  s_context->button()->TurnOn();
  s_context->display()->set(8888);
}

void StartUpState::leave_state() {
  s_context->led()->set_color(mdlib::BLACK);
  s_context->fan()->turnOff();
  s_context->button()->TurnOff();
}

State* StartUpState::loop() {
  unsigned long elapsed = millis() - start_time_;

  if (elapsed >= DURATION)
    return next_state_;

  int h = (int)(360.0f * (float)elapsed / (float)DURATION);
  s_context->led()->set_hsv(h, 1.0f, 1.0f);
  return (State*)0;
}

State* StartUpState::handle_event(mdlib::Event e) {
  return (State*)0;
}

/////// WarmUpState
void WarmUpState::enter_state() {
  State::enter_state();
  s_context->sensor()->turnOn();
  s_context->button()->TurnOff();
}

State* WarmUpState::loop() {
  unsigned long elapsed = millis() - start_time_;

  if (elapsed >= 3000)
    return next_state_;

  int n = 100 - (int)(100.0f * (float)elapsed / 3000.0f);
  s_context->display()->set(n);
  delay(10);
  return (State*)0;
}

/////////////////// ReadyState

  void ReadyState::enter_state() {
    State::enter_state();

    s_context->sensor()->turnOn();
    s_context->button()->TurnOff();
    s_context->display()->clear();
    s_context->fan()->turnOff();
  }

  State* ReadyState::loop() {
    unsigned long elapsed = millis() - start_time_;

    // make the button blink
    float secs =  floor((float)elapsed/1000.0f);
    if( (int)secs % 2) {
      float fraction = (float)elapsed/1000.0f - secs;
      if (fraction < 0.1)
	s_context->button()->TurnOn();
      else
	s_context->button()->TurnOff();
    }
    else
      s_context->button()->TurnOff();

    return (State*)0;
  }

  State* ReadyState::handle_event(mdlib::Event e) {
    // advance to sampling if button clicked
    Serial.print("Got event: ");
    Serial.println(event_name(e));
    if (e.event_type == mdlib::Event::BUTTON_CLICK) {
      Serial.println("BUTTON_CLICK");
      return next_state_;
    }

    return (State*)0;
  }

  ///////////////////// SamplingState

  
} // namespace BOM
