#include <Arduino.h>

class LiquidCrystal_SR3W;
class LcdDisplayMode;
class SAC_ClockDisplay;

/**
 * The LCD portion of the Display Module.
 */
class SAC_LcdDisplay {
 public:
  // XXX Update nModes initialization when adding Modes
  static const int nModes;
  enum Mode {BANNER, TIME, TEMP, VOLTAGES, PRESSURE};
  
  SAC_LcdDisplay(int data, int clock, int latch,
		 int red, int green, int blue,
		 SAC_ClockDisplay* clockDisplay);

  // void SetTime(); - gets it from the clock display
  void SetTemperature(short temperature);
  void SetPressure(long pressure);
  void SetVoltages(float voltage1, float voltage2, float voltage3);

  void setup();

  void SetColor(byte r, byte g, byte b);

  void Update();
  
 private:
  void StartMode(Mode m);
  
  Mode mCurrentMode;
  
  void SetColor(int c);
  
  struct LcdDisplayModeEntry {
    LcdDisplayMode* mode;
    Mode next;
    int color;
    bool modified;
  };
  LcdDisplayModeEntry* mMode;
  
  LiquidCrystal_SR3W* mLcd;
  int mRed;
  int mGreen;
  int mBlue;
};
