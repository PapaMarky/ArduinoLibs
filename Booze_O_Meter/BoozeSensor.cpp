// Copyright (c) 2013 Mark Dyer. All rights reserved.

#include <Arduino.h>

#include "BoozeSensor.h"
#include "EventQueue.h"

namespace BOM {
BoozeSensor::BoozeSensor()
{
}

void BoozeSensor::TakeSample() {
  int sample = RawAlcoholValue();
  data_window_.AddSample((float)sample);
  thermistor_window_.AddSample((float)RawThermistor());
  last_sample_time_ = millis();

  if (recording_ && sample > maximum_reading_) {
    maximum_reading_ = sample;
    PostEvent(mdlib::Event(BOOZE_MAX_CHANGED, (int)this));
#if 0 // turn off debugging
    Serial.print("NEW MAX: ");
    Serial.println(maximum_reading_);
#endif 
  }
#if 0 // turn off debugging
  if (false && data_window_.IsReady()) {
    Serial.print("Data ");
    Serial.print(data_window_.GetLastSample());
    if (data_window_.IsRising())
      Serial.print(" Rising,  ");
    else if (data_window_.IsFalling())
      Serial.print(" Falling, ");
    else if (data_window_.IsStable())
      Serial.print(" STABLE,  ");
    else
      Serial.print(" ?????,   ");

    Serial.print("Temp ");
    Serial.print(thermistor_window_.GetLastSample());
    if (thermistor_window_.IsRising())
      Serial.print(" Rising:  ");
    else if (thermistor_window_.IsFalling())
      Serial.print(" Falling: ");
    else if (thermistor_window_.IsStable())
      Serial.print(" STABLE:  ");
    else
      Serial.print(" ?????: ");

    Serial.print(" -> ");
    Serial.println(thermistor_window_.StandardDeviation(), 2);
    Serial.println("----------------");
  }
#endif
}

void BoozeSensor::TurnOn() {
  control_.TurnOn();
  on_time_ = millis();
  data_window_.Reset();
  thermistor_window_.Reset();
}

void BoozeSensor::TurnOff() {
  control_.TurnOff();
}

void BoozeSensor::update() {
  if (IsOn()) {
    unsigned int elapsed = millis() - last_sample_time_;
    if (elapsed >= 400)
      TakeSample();
  }
}
  
float BoozeSensor::CalculateAlcoholPercent() const {
  float sensor_value = (float)data_.read();

  //  if (sensor_value <= alcohol_sensor_zero)
  //    return 0.0;

  //  if (sensor_value >= alcohol_sensor_100)
  //    return 100.0;

  //  return ((sensor_value - alcohol_sensor_zero) / (alcohol_sensor_100 - alcohol_sensor_zero)) * 100.0;
  return 0.0;
}

void BoozeSensor::set_pins(int control, int data, int temperature) {
  control_.set_pin(control);
  data_.set_pin(data);
  thermistor_.set_pin(temperature);
}

int BoozeSensor::getTemperature() const {
  int raw = thermistor_.read();

  float pad = 9850;
  
  float Temp;  // Dual-Purpose variable to save space.

  long Resistance=((1024 * pad / raw) - pad); 
  Temp = log(Resistance); // Saving the Log(resistance) so not to calculate  it 4 times later
  Temp = 1 / (0.001129148 + (0.000234125 * Temp) + (0.0000000876741 * Temp * Temp * Temp));
  Temp = Temp - 273.15;  // Convert Kelvin to Celsius                      
  //temp = (Temp * 9.0)/ 5.0 + 32.0;                  // Convert to Fahrenheit
  return (int)(Temp * 10.0);                                      // Return the Temperature
}

} // namespace BOM
