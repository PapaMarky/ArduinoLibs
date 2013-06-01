// Copyright (c) 2013 Mark Dyer. All rights reserved.

#include <Arduino.h>

#include "BoozeSensor.h"

namespace BOM {
BoozeSensor::BoozeSensor()
{
}

// observed values
const float alcohol_sensor_zero = 300;
const float alcohol_sensor_100 = 1023;

void BoozeSensor::TakeSample() {
  data_window_.AddSample((float)RawAlcoholValue());
  thermistor_window_.AddSample((float)RawThermistor());
  last_sample_time_ = millis();
}

void BoozeSensor::turnOn() {
  control_.turnOn();
  on_time_ = millis();
  data_window_.Reset();
  thermistor_window_.Reset();
  TakeSample();
}

void BoozeSensor::turnOff() {
  control_.turnOff();
}

void BoozeSensor::update() {
  if (isOn()) {
    unsigned int elapsed = millis() - last_sample_time_;
    if (elapsed >= 400) {
      TakeSample();

      if (data_window_.IsReady()) {
	Serial.print("DATA: ");
	data_window_.Dump();
	Serial.print("TEMP: ");
	thermistor_window_.Dump();
      }
    }
  }
}
  
float BoozeSensor::CalculateAlcoholPercent() const {
  float sensor_value = (float)data_.read();

  if (sensor_value <= alcohol_sensor_zero)
    return 0.0;

  if (sensor_value >= alcohol_sensor_100)
    return 100.0;

  return ((sensor_value - alcohol_sensor_zero) / (alcohol_sensor_100 - alcohol_sensor_zero)) * 100.0;
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
