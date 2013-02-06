#include <SwissArmyClock.h>
#include <Wire.h>

#include "DisplayModule.h"


void DisplayModule::SetTemperature(short temperature) {
  Debug(" SET_TEMP: ");
  DebugLn(temperature);
  Wire.beginTransmission(display_address);
  Wire.write(SET_TEMP);
  delay(1);

  SendValue(&temperature, sizeof(short));

  byte et = Wire.endTransmission();
}


void DisplayModule::SetPressure(long pressure) {
  Debug(" SET_PRESSURE: ");
  DebugLn(pressure);
  Wire.beginTransmission(display_address);
  Wire.write(SET_PRESSURE);
  delay(1);

  SendValue(&pressure, sizeof(long));

  byte et = Wire.endTransmission();
}

void DisplayModule::SetBrightness(int brightness) {
  Debug(" SET_BRIGHTNESS: ");
  DebugLn(brightness);
  Wire.beginTransmission(display_address);
  Wire.write(SET_BRIGHTNESS);
  delay(1);

  SendValue(&brightness, sizeof(int));

  byte et = Wire.endTransmission();
}

void DisplayModule::SetTime(uint32_t time)
{
  Wire.beginTransmission(display_address);
  // Send the command
  Wire.write(SET_TIME);
  delay(1);
  Debug("SetTime : sending ("); Serial.print(SET_TIME, HEX); Serial.print(") ");
  DebugLn(time, HEX);

  SendValue(&time, sizeof(uint32_t));

  byte et = Wire.endTransmission();
  Debug("endTransmission returns ");
  DebugLn(et);
}

void DisplayModule::SendValue(void* v, int size) 
{
  byte* bPtr = (byte*)v;
  for(int i = 0; i < size; i++) {
    Wire.write(bPtr[i]);
    delay(1);
  }
}

/**
 * Set the voltages to display.
 * v1 - main processor voltage
 * v2 - display processor voltage
 * v3 - LED display voltage
 */
void DisplayModule::SetVoltages(float v1, float v2, float v3)
{
  Wire.beginTransmission(display_address);

  Wire.write(SET_VOLTAGES);
  delay(1);

  SendValue(&v1, sizeof(float));
  SendValue(&v2, sizeof(float));
  SendValue(&v3, sizeof(float));

  Wire.endTransmission();
}

DisplayModule::status DisplayModule::GetStatus()
{
  status Status = ERROR;
  byte buffer[8];

  int r = Wire.requestFrom(display_address, 1);
  delay(10);
  int i = 0;
  while(Wire.available() > 0 && i < 1) {
    buffer[i++] = Wire.read();
    delay(1);
  }
  if(i == 1) {
    switch(buffer[0]) {
    case STARTUP:
      Status = DisplayModule::STARTUP;
      break;
    case TIMESET:
      Status = DisplayModule::RUNNING;
      break;
    }
  }
  return Status;
}
