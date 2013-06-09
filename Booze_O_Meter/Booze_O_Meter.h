// Copyright (c) 2013 Mark Dyer. All rights reserved.
#ifndef BOOZE_O_METER_H__
#define BOOZE_O_METER_H__

#include "../base/base.h"
#include "MultiColorLED.h"
#include "Button.h"
#include "StateMachine.h"
#include "StateContext.h"

#include "BoozeSensor.h"

#include <SoftwareSerial.h>

namespace BOM {

/**
 * The internal representation of the booze_o_meter.
 *
 * Runs on the chip inside the booze-o-meter.
 *
 * Parts List:
 * - Fan Control (fan_)
 * - Gas Sensor (Control)
 * - Gas Sensor (Data)
 * - RGB LED (3 AnalogOut pins)
 * - 4 Digit LED Display
 * - I2C clock
 * - I2C data
 * - Main Button
 * - Up Button
 * - Down Button
 */

class Booze_O_Meter {
 public:
  // constructor from 3rd party lib SoftwareSerial used for display_
  // requires the pins in the ctor, so we have to pass them along from
  // ours.
  Booze_O_Meter();

  ~Booze_O_Meter();

  void set_context(StateContext* context);

  void setup();
  void update();

 private:
  // concrete state machines
  static StartUpState START_UP;
  static WarmUpState WARM_UP;
  static ReadyState READY;
  static SamplingState SAMPLING;
  static PostSampleState POST_SAMPLE;
  static PostSample2State POST_SAMPLE_2;
  static SleepState SLEEP;
  static PowerSaverState POWER_SAVER;

  StateContext* context_;

  mdlib::DigitalInput i2c_jumper_;

  void set_state(State* state) {
    if (state_) {
      state_->leave_state();
    }
    state_ = state;
    state_start_millis_ = millis();
    state_->enter_state();
#if 0
    Serial.print("set_state(\"");
    Serial.print(state_->name());
    Serial.println("\")");
#endif
  }
  State* state_;
  unsigned long state_start_millis_;
};
}

#endif // BOOZE_O_METER_H__

