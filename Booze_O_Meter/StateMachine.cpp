// Copyright (c) 2013 Mark Dyer. All rights reserved.

#include <Arduino.h>
#include <Narcoleptic.h>
#include "../base/base.h"
#include "BoozeSensor.h"
#include "LightedButton.h"
#include "MultiColorLED.h"
#include "SevenSegmentDisplay.h"
#include "StateContext.h"
#include "StateMachine.h"

namespace mdlib {
  extern const char* event_name(mdlib::Event e);
}

namespace BOM {
StateContext* State::s_context = 0;

void State::enter_state() {
  SetStartTime();
}

////////////////////////////////////////////////////////////////////
//      StartUpState
////////////////////////////////////////////////////////////////////

void StartUpState::enter_state() {
  SetStartTime();

  s_context->led()->set_color(mdlib::WHITE);
  s_context->fan()->TurnOn();
  s_context->sensor()->TurnOn();
  s_context->button()->TurnOn();
  s_context->display()->Reset();
  s_context->display()->SetBrightness(254);
  s_context->display()->set(8888);
  
}

void StartUpState::leave_state() {
  s_context->led()->set_color(mdlib::BLACK);
  s_context->fan()->TurnOff();
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
  SetStartTime();
  s_context->sensor()->TurnOn();
  s_context->fan()->TurnOff();
  s_context->button()->SetBrightness(0.5);
  s_context->button()->TurnOff();
  s_context->display()->clear();
  s_context->display()->SetBrightness(245);
  display_value_ = 0.0;
  pulse_start_ = millis();
}

void WarmUpState::leave_state() {
  // the user may have double clicked the button and turned the fan on,
  // make sure it is off
  s_context->fan()->TurnOff();
}

State* WarmUpState::loop() {
  const unsigned long PULSE_TIME = 450;
  const unsigned long GAP_TIME = 1550;
  unsigned long elapsed = millis() - pulse_start_;

  if (elapsed > PULSE_TIME + GAP_TIME) {
    pulse_start_ = millis();
    elapsed = 0;
  }

  float v;

  if (elapsed < PULSE_TIME) {
    v = 1.0 - (float)elapsed / (float)PULSE_TIME;
    v *= v;
    s_context->led()->set_hsv(240, 1.0, v);
  }
  else {
    s_context->led()->set_hsv(240, 1.0, 0.005);
  }

  v = s_context->sensor()->DataStdDev();
  if (v != display_value_) {
    display_value_ = v;
    s_context->display()->set(display_value_, 2);
  }

  if (s_context->sensor()->IsReady())
   return next_state_;

  return (State*)0;
}
  State* WarmUpState::handle_event(mdlib::Event e) {
    if (e.event_type == mdlib::Event::BUTTON_DOUBLE_CLICK) {
      if (s_context->fan()->IsOn())
	s_context->fan()->TurnOff();
      else
	s_context->fan()->TurnOn();
    }
    return 0;
  }
/////////////////// ReadyState

  void ReadyState::enter_state() {
    SetStartTime();
    StartTimer();

    s_context->sensor()->TurnOff();
    s_context->sensor()->TurnOn();
    s_context->button()->SetBrightness(0.5);
    s_context->button()->TurnOff();
    s_context->display()->clear();
    s_context->display()->SetBrightness(250);
    s_context->fan()->TurnOff();
    s_context->led()->set_hsv(120, 1.0, 0.1);
  }

  void ReadyState::leave_state() {
    StopTimer();
  }

  State* ReadyState::loop() {
    UpdateTimer();
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

    // count down display
    static unsigned long t = 999999;
    float time_left = timer_.GetSecondsRemaining();
    if ((int)time_left != t) {
      t = time_left;
      s_context->display()->SetSeconds(t);
    }
#if 0
    // Make the led Pulse green

    static int rise_count = 0;
    if (s_context->sensor()->IsRising()) {
      rise_count++;
      if (rise_count > 5)
	return next_state_;
    }
    else
      rise_count = 0;
#endif 
    return (State*)0;
  }

  State* ReadyState::handle_event(mdlib::Event e) {
    // advance to sampling if button clicked
    if (e.event_type == mdlib::Event::BUTTON_CLICK) {
      return next_state_;
    }
    else if (e.event_type == mdlib::Event::TIMER_DONE) {
      return timeout_next_state_;
    }

    return (State*)0;
  }

  ///////////////////// SamplingState
  static int hue = 0;

  void SamplingState::enter_state() {
    SetStartTime();
    StartTimer();
    hue = 0;
    s_context->display()->clear();
    s_context->display()->set(hue);
    s_context->display()->SetBrightness(254);
    s_context->led()->set_hsv(hue, 1.0f,  1.0f);
    s_context->sensor()->StartRecording();

    start_sample_ = s_context->sensor()->RawAlcoholValue();
  }

  void SamplingState::leave_state() {
    StopTimer();
  }

  State* SamplingState::handle_event(mdlib::Event e) {
    static bool fanIsOn = false;

    if (e.event_type == mdlib::Event::TIMER_DONE) {
      return timeout_next_state_;
    }

    if (e.event_type == mdlib::Event::BUTTON_CLICK) {
      return next_state_;
    }

    if (e.event_type == BoozeSensor::BOOZE_MAX_CHANGED) {
      UpdateDisplay();
      StartTimer();
      // If we peg the meter, might as well move along
      if (s_context->sensor()->GetMaximum() == 1023)
        return next_state_;
    }
    return 0;
  }

  int hueMap(float x) {
    const int GREEN = 120;
    const int RED = 0;
    
    float in_min = 0.25; // below 25% is green
    float in_max = 0.90; // above 90% is red

    float out_min = GREEN; // 
    float out_max = RED;

    if (x <= in_min)
      return (int)out_min;
    
    if (x >= in_max)
      return (int)out_max;

    return (int)( (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min );
  }

  void SamplingState::UpdateDisplay() {
    int sample = s_context->sensor()->GetMaximum();

    float pct = (float)(sample - start_sample_) / (float)(1023 - start_sample_);
    int hue = hueMap(pct);

    s_context->led()->set_hsv(hue, 1.0f, 1.0f);
    s_context->display()->set((int)(pct * 100.0));
  }

  State* SamplingState::loop() {
    UpdateTimer();
    return 0;
  }
  ///////////////////////// PostSampleState

  void PostSampleState::enter_state() {
    SetStartTime();
    StartTimer();
    s_context->display()->SetBrightness(254);
    s_context->fan()->TurnOn();
    s_context->button()->TurnOff();
  }

  void PostSampleState::leave_state() {
    StopTimer();
  }

  State* PostSampleState::handle_event(mdlib::Event e) {
    if (e.event_type == mdlib::Event::TIMER_DONE)
      return timeout_next_state_;

    if (e.event_type == mdlib::Event::BUTTON_CLICK)
      return next_state_;
  }

  State* PostSampleState::loop() {
    // blink the display
    UpdateTimer();
  }

  ///////////////////////// PostSampleState2

  void PostSample2State::enter_state() {
    SetStartTime();
    StartTimer();
    s_context->display()->SetBrightness(254);
    s_context->fan()->TurnOff();
    s_context->button()->TurnOff();
  }

  void PostSample2State::leave_state() {
    StopTimer();
  }

  State* PostSample2State::handle_event(mdlib::Event e) {
    if (e.event_type == mdlib::Event::TIMER_DONE)
      return timeout_next_state_;

    if (e.event_type == mdlib::Event::BUTTON_CLICK)
      return next_state_;
  }

  ///////////////// PowerSaverState

  void PowerSaverState::enter_state() {
    SetStartTime();
    StartTimer();
    
    s_context->fan()->TurnOff();

    s_context->led()->set_hsv(120, 1.0, 0.005);
    s_context->button()->SetBrightness(0.5);
    s_context->display()->SetBrightness(200);
  }

  void PowerSaverState::leave_state() {
    StopTimer();
  }

  State* PowerSaverState::handle_event(mdlib::Event e) {
    if (e.event_type == mdlib::Event::BUTTON_CLICK)
      return next_state_;

    if (e.event_type == mdlib::Event::TIMER_DONE)
      return timeout_next_state_;
  }

  // LOOP
  // blink the button and led dimmer & with longer off cycle
  State* PowerSaverState::loop() {
    UpdateTimer();
    unsigned long elapsed = millis() - start_time_;
    // make the button blink
    float secs =  floor((float)elapsed/2000.0f);
    if( (int)secs % 2) {
      float fraction = (float)elapsed/2000.0f - secs;
      if (fraction < 0.05)
	s_context->button()->TurnOn();
      else
	s_context->button()->TurnOff();
    }
    else
      s_context->button()->TurnOff();

    // count down display
    static int t = 999999;
    float time_left = timer_.GetSecondsRemaining();

    if ((int)time_left != t) {
      t = time_left;
      s_context->display()->SetSeconds(t);
    }
    
    return 0;
  }
///////////////////////// SleepState

  void SleepState::enter_state() {
    s_context->led()->TurnOff();
    s_context->button()->TurnOff();
    s_context->sensor()->TurnOff();
    s_context->display()->clear();
    s_context->display()->set("    ");
  }

  State* SleepState::handle_event(mdlib::Event e) {
    if (e.event_type == mdlib::Event::BUTTON_CLICK ||
	e.event_type == mdlib::Event::BUTTON_LONG_CLICK ||
	e.event_type == mdlib::Event::BUTTON_DOWN	||
	e.event_type == mdlib::Event::BUTTON_UP	)
      return next_state_;
  }

  State* SleepState::loop() {
    Narcoleptic.delay(2000);
    return 0;
  }
} // namespace BOM

