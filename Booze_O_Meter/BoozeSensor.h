// Copyright (c) 2013 Mark Dyer. All rights reserved.
#ifndef BOOZE_SENSOR__
#define BOOZE_SENSOR__

#include "../base/base.h"
#include "SlidingDataWindow.h"

namespace BOM {
class BoozeSensor {
 public:
  BoozeSensor();

  void set_pins(int control, int data, int temperature);
  
  void turnOn() { control_.turnOn(); }
  void turnOff() { control_.turnOff(); }

  bool isOn() const { return control_.getState(); }
  
  void set_state(bool b) { control_.set_state(b); }

  int getTemperature() const;
  
  int read() const {
    return data_.read();
  }

  void setup()  {
    control_.setup();
    data_.setup();
    thermistor_.setup();
    data_window_.SetWindowSize(25);
    data_window_.SetStableSize(1.0);
    thermistor_window_.SetWindowSize(25);
    thermistor_window_.SetStableSize(1.0);
  }

  float CalculateAlcoholPercent() const;
  int RawAlcoholValue() const { return data_.read(); }
  
 private:
  mdlib::DigitalOutput control_;
  mdlib::AnalogInput data_;
  mdlib::AnalogInput thermistor_;

  mdlib::SlidingDataWindow data_window_;
  mdlib::SlidingDataWindow thermistor_window_;
};
} // namespace BOM
#endif // BOOZE_SENSOR__

