// Copyright (c) 2013 Mark Dyer. All rights reserved.
#ifndef BOOZE_O_METER_H__
#define BOOZE_O_METER_H__

#include "../base/base.h"
#include "MultiColorLED.h"
#include "Button.h"

#include <SoftwareSerial.h>

class BoozeSensor {
 public:
  BoozeSensor();

  void set_pins(int control, int data, int temperature);
  
  void turnOn() { control_.turnOn(); }
  void turnOff() { control_.turnOff(); }

  bool isOn() { return control_.getState(); }
  
  void set_state(bool b) { control_.set_state(b); }

  int getTemperature();
  
  int read() {
    return data_.read();
  }

  void setup() {
    control_.setup();
    data_.setup();
    thermistor_.setup();
  }

  float CalculateAlcoholPercent();
  int RawAlcoholValue() { return data_.read(); }
  
 private:
  mdlib::DigitalOutput control_;
  mdlib::AnalogInput data_;
  mdlib::AnalogInput thermistor_;  
};

/**
 * The internal representation of the booze_o_meter.
 *
 * Runs on the chip inside the booze-o-meter.
 *
 * Parts List:
 * - Standalone Jumper (i2c_jumper_)
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
  Booze_O_Meter(int display_rx, int display_tx);

  ~Booze_O_Meter();

  void set_fan_pin(int pin) { fan_.set_pin(pin); }
  void set_main_button_pin(int pin) { main_button_.set_pin(pin); }
  void set_up_down_button_pins(int up, int down) {
    up_button_.set_pin(up);
    down_button_.set_pin(down);
  }
  void set_booze_sensor_pins(int control, int data, int temperature) {
    sensor_.set_pins(control, data, temperature);
  }

  void set_rgb_led_pins(int red_pin, int blue_pin, int green_pin) {
    rgb_led_.set_pins(red_pin, blue_pin, green_pin);
  }

  bool isStandalone() { return standalone_; }

  void setup();
  void loop();

 private:
  bool standalone_;
  enum State {
    POWER_ON,
    CALIBRATION,
  };
  
  mdlib::DigitalOutput fan_;
  mdlib::DigitalInput i2c_jumper_;
  SoftwareSerial display_;
  mdlib::Button main_button_;
  mdlib::Button up_button_;
  mdlib::Button down_button_;
  mdlib::MultiColorLED rgb_led_;

  bool button_states_[3];
  BoozeSensor sensor_;
  

  void set_state(State state) {
    state_ = state;
    state_start_millis_ = millis();
  }
  State state_;
  unsigned long state_start_millis_;

  // loop functions for each State
  void HandleEvents();

  void power_on_loop();
  void calibration_loop();
};

#endif // BOOZE_O_METER_H__

