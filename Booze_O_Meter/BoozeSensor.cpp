// Copyright (c) 2013 Mark Dyer. All rights reserved.

#include "BoozeSensor.h"
namespace BOM {
BoozeSensor::BoozeSensor()
{
}

void BoozeSensor::set_pins(int control, int data, int temperature) {
  control_.set_pin(control);
  data_.set_pin(data);
  thermistor_.set_pin(temperature);
}

int BoozeSensor::getTemperature() {
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
