// Copyright (c) 2013 Mark Dyer. All rights reserved.
#ifndef BOOZE_SENSOR__
#define BOOZE_SENSOR__

#include "../base/base.h"

namespace BOM {
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
} // namespace BOM
#endif // BOOZE_SENSOR__

