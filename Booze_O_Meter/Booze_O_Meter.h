#ifndef BOOZE_O_METER_H__
#define BOOZE_O_METER_H__

#include "../base/base.h"

#include <SoftwareSerial.h>

class BoozeSensor {
 public:
  BoozeSensor(int control_pin, int data_pin, int temperature_pin);

  void turnOn() { control_.turnOn(); }
  void turnOff() { control_.turnOff(); }

  bool isOn() { return control_.getState(); }
  
  void set_state(bool b) { control_.set_state(b); }

  int getTemperature() {
    return thermistor_.read();
  }
  
  int read() {
    return data_.read();
  }

  void setup() {
    data_.setup();
  }
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
  Booze_O_Meter(int fan_pin,
		int i2c_jumper_pin,
		int disp_rx_pin,
		int disp_tx_pin,
		int main_button_pin,
		int up_button_pin,
		int down_button_pin,
		int sensor_control_pin,
		int sensor_data_pin,
		int sensor_temperature_pin);

  ~Booze_O_Meter();

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
  mdlib::MomentaryButton main_button_;
  mdlib::MomentaryButton up_button_;
  mdlib::MomentaryButton down_button_;
  bool button_states_[3];
  BoozeSensor sensor_;
  
  State state_;

  // loop functions for each State
  void power_on_loop();
  void calibration_loop();
};

#endif // BOOZE_O_METER_H__

