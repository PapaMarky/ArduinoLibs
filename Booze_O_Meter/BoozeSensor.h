// Copyright (c) 2013 Mark Dyer. All rights reserved.
#ifndef BOOZE_SENSOR__
#define BOOZE_SENSOR__

#include "../base/base.h"
#include "SlidingDataWindow.h"

namespace BOM {
class BoozeSensor {
 public:
  // event id
  static const int BOOZE_MAX_CHANGED = 5000;
  BoozeSensor();

  void set_pins(int control, int data, int temperature);

  void TurnOn();
  void TurnOff();

  bool IsOn() const { return control_.getState(); }

  bool IsReady() const {
    return IsOn() && data_window_.IsStable() && thermistor_window_.IsStable();
  }

  bool IsRising() const {
    return IsOn() && data_window_.IsRising();
  }

  void set_state(bool b) { control_.set_state(b); }

  int getTemperature() const;
  int RawThermistor() { return thermistor_.read(); }
  int read() const {
    return data_.read();
  }

  void update();
  
  void setup()  {
    control_.setup();
    data_.setup();
    thermistor_.setup();
    data_window_.SetStableSize(0.8);
    data_window_.SetTrendSampleSize(5);
    thermistor_window_.SetStableSize(1.5);
    thermistor_window_.SetTrendSampleSize(5);
  }

  float CalculateAlcoholPercent() const;
  int RawAlcoholValue() const { return data_.read(); }

  float DataStdDev() { return data_window_.StandardDeviation(); }

  void StartRecording() { maximum_reading_ = 0; recording_ = true; }
  void StopRecording() { recording_ = false; }
  int GetMaximum() { return maximum_reading_; }
 private:
  void TakeSample();

  mdlib::DigitalOutput control_;
  mdlib::AnalogInput data_;
  mdlib::AnalogInput thermistor_;

  mdlib::SlidingDataWindow data_window_;
  mdlib::SlidingDataWindow thermistor_window_;

  unsigned int on_time_;
  unsigned int last_sample_time_;

  bool recording_;
  int maximum_reading_;
};
} // namespace BOM
#endif // BOOZE_SENSOR__

