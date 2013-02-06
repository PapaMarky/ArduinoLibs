#include <Arduino.h>

class LiquidCrystal_SR3W;

class LcdDisplayMode {
 public:

  /**
   * Update the LCD device
   * Returns true if this mode should end
   */
  virtual bool Update() = 0;

  virtual void Start();
  
 protected:
  unsigned long Elapsed();
  
  LcdDisplayMode(LiquidCrystal_SR3W *lcd);
  LiquidCrystal_SR3W *mLcd;
  unsigned long mStartTime;
  bool mInitialized;
  bool mModified;
};

/**
 * Display the Banner for 5 seconds, then exit.
 */
class BannerMode : public LcdDisplayMode {
 public:
  BannerMode(LiquidCrystal_SR3W* lcd);

  virtual void Start();
  virtual bool Update();
 
 private:
};

class SAC_ClockDisplay;
class TimeMode : public LcdDisplayMode {
 public:
  TimeMode(LiquidCrystal_SR3W* lcd, SAC_ClockDisplay* clockDisplay);

  virtual bool Update();

  virtual void Start();
 private:
  SAC_ClockDisplay* mClockDisplay;
  enum MODE {UNKNOWN, CLOCK_NOT_SET, CLOCK_SET};
  MODE mMode;
  int mPrevHour;
  int mPrevMinute;
};

class TemperatureMode : public LcdDisplayMode {
 public:
  TemperatureMode(LiquidCrystal_SR3W* lcd);
  
  void Set(short t) { mTemperature = t; mInitialized = true; mModified = true;}

  virtual bool Update();
  
 private:
  short mTemperature;
};

class PressureMode : public LcdDisplayMode {
 public:
  PressureMode(LiquidCrystal_SR3W* lcd);

  void Set(long p) { mPressure = p; mInitialized = true; mModified = true;}

  virtual bool Update();
  
 private:
  long mPressure;
};

class VoltagesMode : public LcdDisplayMode {
 public:
 private:
  float mVoltage1;
  float mVoltage2;
  float mVoltage3;
};
