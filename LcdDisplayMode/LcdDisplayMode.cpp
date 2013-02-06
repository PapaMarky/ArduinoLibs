#include <LiquidCrystal_SR3W.h>
#include <SAC_ClockDisplay.h>

#include "../SwissArmyClock/SwissArmyClock.h"
#include "LcdDisplayMode.h"

LcdDisplayMode::LcdDisplayMode(LiquidCrystal_SR3W* lcd)
  : mLcd(lcd), mStartTime(0), mInitialized(false), mModified(true)
{
}

void LcdDisplayMode::Start()
{
  mStartTime = millis();
  mModified = true;
}

unsigned long LcdDisplayMode::Elapsed() {
  unsigned long t = millis();
  return (t - mStartTime);
}
/// Banner

BannerMode::BannerMode(LiquidCrystal_SR3W* lcd)
  : LcdDisplayMode(lcd)
{
}

char nihongo[] = {
  B11000110,
  B11001110,
  B11011101,
  B10111010,
  B11011110,
  B00000000
};

void BannerMode::Start()
{
  LcdDisplayMode::Start();
  // set the text
  mLcd->clear();
  mLcd->home();
  mLcd->print("Swiss Army Clock");
  mLcd->setCursor(0,1);
  mLcd->print(nihongo);
  mLcd->print(" :  v1.0 ");

  mInitialized = true;
  mModified = false;
}

bool BannerMode::Update()
{
  return (Elapsed() >= 5000);
}

/// Time Mode

TimeMode::TimeMode(LiquidCrystal_SR3W* lcd, SAC_ClockDisplay* clockDisplay)
  : LcdDisplayMode(lcd), mClockDisplay(clockDisplay), mMode(UNKNOWN)
{
}

void TimeMode::Start() {
  LcdDisplayMode::Start();
  mMode = UNKNOWN;
}

bool TimeMode::Update()
{
  //Debug("TimeMode::Update() elapsed: ");
  //DebugLn(Elapsed());

  bool need_draw = false;
  
  if (Elapsed() < 5000) {
    if(mClockDisplay->clockIsSet()) {
      DateTime dt(mClockDisplay->now());
      if (mMode == CLOCK_SET) {
	need_draw =
	  mPrevHour != dt.hour() || mPrevMinute != dt.minute();
      }
      else {
	mMode = CLOCK_SET;
	need_draw = true;
      }

      if (need_draw) {
	mLcd->clear();
	mLcd->home();
	mLcd->print(SAC_ClockDisplay::Days[dt.dayOfWeek()]); mLcd->print(" ");
	mLcd->print(SAC_ClockDisplay::Months[dt.month()]); mLcd->print(" ");
	if(dt.day() < 10) mLcd->print(" ");
	mLcd->print(dt.day()); mLcd->print(", ");
	mLcd->print(dt.year());
    
	const char* ampm = dt.hour() < 12 ? " AM" : " PM";
	mLcd->setCursor(4,1);
	int h = dt.hour();
	if(h > 12) h -=12;
	if(h == 0) h = 12;
	if(h < 10) mLcd->print(" ");
	mLcd->print(h);
	mLcd->print(":");
	if(dt.minute() < 10) mLcd->print("0");
	mLcd->print(dt.minute());
	mLcd->print(ampm);

	mPrevHour = dt.hour();
	mPrevMinute = dt.minute();
      }
    }
    else {
      if (mMode != CLOCK_NOT_SET) {
	mMode = CLOCK_NOT_SET;
	mLcd->clear();
	mLcd->home();
	mLcd->print("   Time & Date ");
	mLcd->setCursor(0,1);
	mLcd->print("   Unavailable");
      }
    }
    //DebugLn(" **** false ****");
    return false;
  }
  else {
    //DebugLn(" **** true ****");
    return true;
  }
}

/// Temperature Mode

TemperatureMode::TemperatureMode(LiquidCrystal_SR3W* lcd)
  : LcdDisplayMode(lcd)
{
}

char kion[] = {
  0xB7, 0xB5, 0xDD, 0x00
};

bool TemperatureMode::Update() {
  long int elapsed = Elapsed();
  if (elapsed < 5000) {
    mLcd->clear();
    mLcd->home();
    char* label;
    char unit;
    float value;
    if (!mInitialized || (elapsed/2500) % 2 == 0) {
      // Fahrenheit
      label = "Temperature";
      if (mInitialized) {
	value = ((float)mTemperature/10.0) * 9.0/5.0 + 32.0;
	unit = 'F';
      }
    }
    else {
      // Celsius
      label = kion;
      value = (float)mTemperature/10.0;
      unit = 'C';
    }
    mLcd->print(label);
    mLcd->setCursor(0, 1);
    if (mInitialized) {
      mLcd->print(value, 1);
      mLcd->print((char)0xDF);
      mLcd->print(unit);
    }
    else {
      mLcd->print("-- Unavailable");
    }
    return false;
  }
  else {
    return true;
  }
}

/// PressureMode

PressureMode::PressureMode(LiquidCrystal_SR3W* lcd)
  : LcdDisplayMode(lcd)
{
}

bool PressureMode::Update() {
  long int elapsed = Elapsed();
  if (elapsed < 5000) {
    mLcd->clear();
    mLcd->home();
    char* label = "Pressure";

    mLcd->print(label);
    mLcd->setCursor(0, 1);
    if (mInitialized) {
      mLcd->print(mPressure, 1);
    }
    else {
      mLcd->print("-- Unavailable");
    }
    return false;
  }
  else {
    return true;
  }
}
