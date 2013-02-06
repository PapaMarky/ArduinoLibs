#ifndef DISPLAY_MODULE_H
#define DISPLAY_MODULE_H

#include <arduino.h>

class DisplayModule {
 public:
  DisplayModule() {}

  enum status {ERROR, STARTUP, RUNNING};

  void SetTemperature(short temperature);
  void SetPressure(long pressure);
  void SetBrightness(int brightness);
  void SetTime(uint32_t time);
  void SetVoltages(float v1, float v2, float v3);
  
  status GetStatus();

 private:
  void SendValue(void* v, int size);
  
};

#endif // DISPLAY_MODULE_H
